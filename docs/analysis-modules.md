# Module Analysis

Analysis of bugs, potential issues, and code smells in synthesis modules.

## Critical Issues

### 1. Integer Division Bug (MidiMono)

**MidiMono.cpp:6-7**:
```cpp
#define DIV127 1/127  // Evaluates to 0 (integer division)
#define DIV120 1/120  // Evaluates to 0 (integer division)
```
These should be `1.0f/127` or `(1.0/127)`. The current code makes MIDI CV output always 0.

**Impact:** MIDI pitch CV is broken - notes will not track pitch correctly.

### 2. Memory Leak (Looper)

**Looper.cpp:26**:
```cpp
buttons[i] = new Button2t(a, _BTN[i]);
```
Button2t objects are allocated but never deleted.

---

## Bugs

### 3. Looper Counter Bug

**Looper.cpp:143-147**:
```cpp
for (uint j = 0; j < TRACKS_COUNT; j++) {
    if(loops[j].state==QUEUED)
        loops[j].play();
    queuedCount--;  // Decremented even when state != QUEUED
    playingCount++;
}
```
Counters are updated unconditionally, not just when `state==QUEUED`. This corrupts the track state machine.

### 4. Documented Bug (Looper)

**Looper.cpp:161-162**:
```cpp
//////////////////
///known issue: if the longest track is deleted, all the others remain stuck in the queue
//////////////////
```
This is a known unfixed bug that breaks looper functionality.

### 5. Thread Safety in MIDI Callbacks

**MidiMono.cpp:80-97**:
```cpp
static void handleNoteOn(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity) {
    if(inChannel==_channel && inVelocity!=0){
        _gate.amplitude(1);  // Modifies shared state
        _cv.amplitude(...);
    }
}
```
If called from interrupt context, this could race with main loop reading these values.

---

## Missing Error Handling

### 6. Looper File Operations

**Looper.cpp:184**:
```cpp
file = SD.open(filename, FILE_WRITE);  // Could fail
recorder.begin();  // Proceeds regardless
```
No check if file opened successfully. Recording to a failed file handle causes undefined behavior.

**Looper.cpp:206**:
```cpp
void LoopTrack::cancelRec() {
    SD.remove(filename);  // Remove file
    file.close();         // Then close handle
}
```
Order of operations may cause issues; closing an already-removed file.

---

## Code Smells

### 7. Unused Code (VCA)

**VCA.cpp**:
```cpp
AudioEffectMultiply _amp0,  // Declared but never used
                    _amp1;

enum _outputs { OUT0, OUT1};  // OUT0 never used
enum _inputs { OUT0_D, IN0, IN0_D, CV0, CV0_D, ... };  // Many unused
```
Only half the VCA is implemented (channel 1 only, not channel 0).

### 8. Static State in MidiMono

**MidiMono.h:24-28**:
```cpp
static AudioSynthWaveformDc _gate;
static AudioSynthWaveformDc _cv;
static AudioSynthWaveformDc _vel;
static uint_fast8_t _channel;
```
This prevents having multiple MIDI modules with independent channels.

### 9. Empty Implementation (EmptyModule)

**Emptymodule.h:9-10**:
```cpp
//EmptyModule(const Address& a) : Module(a) {};  // Commented out
void updateValues()override{return;};
```
Constructor is commented out, making the class unusable.

### 10. Inconsistent Socket Creation

Some modules use the `Input`/`Output` helper structs:
```cpp
// VCO_det.cpp
Input freqCv;
Output sine;
```

Others directly push to static vectors:
```cpp
// VCA.cpp, MidiMono.cpp
outputSockets.push_back(std::make_shared<OutputSocket>(...));
inputSockets.push_back(std::make_shared<InputSocket>(...));
```

This inconsistency makes code harder to follow.

### 11. Magic Numbers

**Looper.cpp:52-55**:
```cpp
mixer_a.gain(i, 0.4);
mixer_b.gain(i, 0.4);
masterMixer.gain(i, 0.8);
```

**AudioOut.cpp:22-23**:
```cpp
_audioCtrl.volume(0.5);
_volume_pot0.setRange(0, 0.8, EXP);
```

**VCO_det.cpp:57-70**:
```cpp
_sin0.begin(1, 200, WAVEFORM_SINE);  // Why 200Hz default?
_coarse_pot0.setRange(0, 3200, EXP);  // Why 3200?
```

### 12. Hardcoded Pin Mappings

Each module has its own namespace with pin constants:
```cpp
// VCO_det.cpp
namespace {
    enum _outputs { SIN_OUT, TRI_OUT, SAW_OUT, SQR_OUT};
    enum _inputs { SHAPE_CV_D=13, XXX_D=14, FREQ_PHASE_CV_D=24, ... };
};
```
These could be centralized or documented better.

### 13. Commented Debug Code

**MidiMono.cpp:70-76**:
```cpp
//	if (usbSource)
		midiHardware.read();
//	else{
		usbMIDI.read();
		usbHost.Task();
		midiOnUsbHost.read();
//	}
```
The switch functionality is disabled, always reading from all sources.

### 14. Redundant Code (VCO_det)

**VCO_det.cpp:90-97, 102-109, 117-124, 129-136**:
The same operation is repeated 8 times for each oscillator. This should be refactored into a loop or helper function.

```cpp
_sin0.frequency(_freq0);
_tri0.frequency(_freq0);
_saw0.frequency(_freq0);
_sqr0.frequency(_freq0);
_sin1.frequency(_freq1);
_tri1.frequency(_freq1);
_saw1.frequency(_freq1);
_sqr1.frequency(_freq1);
```

---

## Module-Specific Notes

### ADSR
- Uses `AudioRecordQueue` for gate detection which is heavyweight
- Allocates 256-byte buffer on stack every update cycle (line 58)

### Looper
- Serial.printf calls remain in production code (lines 180, 211, 224, 239, 247, 254)
- `overdub()` function is empty (line 261)

### MidiMono
- Velocity output socket exists but velocity value is never set

---

## Summary

| Category | Count |
|----------|-------|
| Critical bugs | 2 |
| Logic bugs | 2 |
| Missing error handling | 2 |
| Code smells | 8 |

**Priority fixes:**
1. Fix integer division in MidiMono (`1.0f/127` instead of `1/127`)
2. Fix Looper counter logic in the play-all loop
3. Add file open error checking in Looper
4. Implement or remove the longest-track deletion bug workaround
5. Complete VCA implementation or remove unused code
