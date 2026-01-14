# Core System Analysis

Analysis of bugs, potential issues, and code smells in the core framework and controls.

## Critical Issues

### 1. Memory Leaks

**ModuleFactory.h:56-141** - Modules allocated with `new` but never deleted:
```cpp
activeModules.push_back(new AudioOut(slotAddress));
```
The `activeModules` vector holds raw pointers that are never freed.

**InputSocket.cpp:24, OutputSocket.cpp:24** - Address objects leaked:
```cpp
: Socket(...), address(new ControlAddress(slotAddress, id))
```
These `new` allocations have no corresponding `delete`.

### 2. Missing Virtual Destructor

**Module.h:26** - Base class has no virtual destructor:
```cpp
class Module{
    // ... no virtual ~Module()
};
```
Since modules are created via `new` and stored as `Module*`, this causes undefined behavior if they were ever deleted.

### 3. Uninitialized Array Elements

**PatchCable.cpp:15-59** - Only some elements of `ac[POLYPHONY]` are initialized:
```cpp
AudioConnection* ac[POLYPHONY]; // 4 elements
// For M2M, M2P, P2M: only ac[0] is set
// Remaining ac[1], ac[2], ac[3] contain garbage pointers
```
If the destructor reads wrong `connectionType`, it could try to delete garbage.

### 4. Duplicate Module Type ID

**ModuleTypeIds.h:24-25**:
```cpp
const static byte FLANGER = 0b00001111;        // 15
const static byte DISTORTION_AMP = 0b00001111; // 15 - SAME VALUE!
```
These collide, making it impossible to distinguish Flanger from Distortion Amp.

---

## Blocking Operations in Real-Time Code

### 5. Blocking delays in controls

**Button.cpp:15-16, 26-29**:
```cpp
bool Button::tap() {
    delay(BOUNCE_DELAY);  // 20ms blocking
}
bool Button::longPress() {
    while (counter > 0) {  // Up to 500ms blocking loop
        counter -= BOUNCE_DELAY;
    }
}
```

**Button2t.cpp:11-13**:
```cpp
delay(DOUBLE_TAP_DELAY);  // 100ms
delay(DOUBLE_TAP_DELAY);  // Another 100ms
```

This blocks the entire audio system, causing audio glitches.

**ModuleTypeIdMux.cpp:16**:
```cpp
delay(MUX_DELAY);  // Called 8 times in loop = 40ms blocking at startup
```

---

## Architectural Issues

### 6. Side Effects in Constructors

**Module.h:48-72** - `Input` and `Output` structs:
```cpp
inline Input(...) {
    base = std::make_shared<InputSocket>(...);
    Module::inputSockets.push_back(base); // Side effect!
}
```
Constructor modifies global state, making object creation non-idempotent and hard to test.

### 7. Global/Static State Overuse

Multiple classes rely on global/static state:
- `main.cpp:36`: `std::vector<Module*> activeModules`
- `main.cpp:37`: `ILI9341_t3 tft`
- `Module.h:38-39`: `static std::vector<InputSocket>`, `outputSockets`
- `PatchCable.h:19`: `static std::list<PatchCable> activeCables`

This creates hidden dependencies and makes unit testing nearly impossible.

### 8. Rule of Three Violation

**InputSocket.h, OutputSocket.h** - Classes with raw pointer members (`address`) have no:
- Copy constructor
- Copy assignment operator
- Move constructor/assignment

Default copy would create double-free bugs.

### 9. Potential Iterator Invalidation

**PatchCable.cpp:159**:
```cpp
activeCables.push_back(std::make_unique<PatchCable>(*out, *(in++)));
```
The `PatchCable` constructor calls `InputSocket::setBusy(in)` which modifies `availableInputs` while we're iterating over it.

---

## Code Smells

### 10. Type Confusion

**Button2t.cpp:28**:
```cpp
detectedTapCount = false; // Should be = 0
```
Assigns boolean `false` to an `int`.

### 11. Unimplemented Scale Modes

**Potentiometer.cpp:43-46**:
```cpp
case LOG:
    return 0;  // Not implemented
case POW:
    return 0;  // Not implemented
```

### 12. Unused Members

- **Module.h:32**: `verbose` member never used
- **Address.h**: Public `_id` alongside getter `toInt()` - inconsistent

### 13. Const-Correctness Issues

**Socket.h:53-59** - These methods have side effects but don't indicate it:
```cpp
inline bool hasJack() { return !jackDetector.b_read(); }
inline bool jackDetectorChanged() { return jackDetector.wasUpdated(); }
```
They internally call update functions that modify state.

### 14. Poor Encapsulation

- `Socket::isPolyphonic` public
- `Socket::uid` public
- `PatchCable::inputSocket`, `outputSocket` public
- `Address::_id` public

### 15. Inconsistent Naming

Mix of styles throughout:
- `_coarse_pot0` vs `att_pot1` vs `source_sw0`
- `uint` vs `uint_fast8_t` vs `int`

### 16. Magic Numbers

- **main.cpp:73**: `AudioMemory(1500)` - why 1500?
- **main.cpp:75**: `delay(2000)` - arbitrary startup delay
- **HardwareCfg.h:125**: `MUX_DELAY 5` - units unclear

---

## Testability Problems

1. No dependency injection - everything uses globals
2. Hardware access mixed with business logic
3. No interfaces/abstractions for mocking
4. Side effects in constructors
5. Static state prevents test isolation

---

## Summary

| Category | Count |
|----------|-------|
| Critical | 4 |
| Blocking operations | 3 |
| Architectural issues | 4 |
| Code smells | 7 |

**Priority fixes:**
1. Add virtual destructor to Module base class
2. Replace blocking delays with non-blocking state machines
3. Fix duplicate ModuleTypeIds (FLANGER/DISTORTION_AMP)
4. Initialize PatchCable::ac array to nullptr
5. Delete or use smart pointers for Address allocations
