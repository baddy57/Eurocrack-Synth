# Static Services Proposal

Organize scattered global/static state into cohesive service classes. No interfaces, no injection, no runtime cost - just better organization.

## Current State (Scattered)

```
main.cpp
├── std::vector<Module*> activeModules
├── ILI9341_t3 tft
├── USBHost usbHost
└── MIDIDevice midiOnUsbHost

Module.h
├── static std::vector<InputSocket> inputSockets
├── static std::vector<OutputSocket> outputSockets
└── static AudioControlSGTL5000 _audioCtrl

PatchCable.h
└── static std::list<PatchCable> activeCables

InputSocket.h
├── static std::list<InputSocket> availableInputs
└── static std::list<InputSocket> busyInputs

OutputSocket.h
└── static std::list<OutputSocket> availableOutputs

MidiMono.h
├── static AudioSynthWaveformDc _gate, _cv, _vel
└── static uint_fast8_t _channel

LoopTrack (in Looper.h)
└── static int longest
```

**Problems:**
- Hard to know "where does X live?"
- Related state split across files
- Socket state in 3 different classes
- Module base class owns unrelated things (_audioCtrl)

---

## Proposed: Static Services

### services/Connections.h
```cpp
#pragma once
#include <vector>
#include <list>
#include <memory>

class InputSocket;
class OutputSocket;
class PatchCable;

// Unified service for sockets and cables - both manage connections
class Connections {
    // Active patch cables
    static std::list<std::unique_ptr<PatchCable>> cables;

    // Internal helpers
    static void searchForCablesToAdd();
    static bool checkPhysicalConnection(std::shared_ptr<OutputSocket> out,
                                        std::shared_ptr<InputSocket> in);

public:
    // All registered sockets
    static std::vector<std::shared_ptr<InputSocket>> inputs;
    static std::vector<std::shared_ptr<OutputSocket>> outputs;

    // State buckets for connection matching
    static std::list<std::shared_ptr<InputSocket>> availableInputs;
    static std::list<std::shared_ptr<InputSocket>> busyInputs;
    static std::list<std::shared_ptr<OutputSocket>> availableOutputs;

    // Socket registration (called by modules at construction)
    static void registerInput(std::shared_ptr<InputSocket> socket);
    static void registerOutput(std::shared_ptr<OutputSocket> socket);

    // Jack events (called when physical jack state changes)
    static void onInputConnected(std::shared_ptr<InputSocket> socket);
    static void onInputDisconnected(std::shared_ptr<InputSocket> socket);
    static void onOutputConnected(std::shared_ptr<OutputSocket> socket);
    static void onOutputDisconnected(std::shared_ptr<OutputSocket> socket);

    // Main loop update - checks all jack states
    static void update();
};
```

### services/SynthAudio.h
```cpp
#pragma once
#include <Audio.h>

class SynthAudio {
public:
    static AudioControlSGTL5000 codec;

    static void init(int memoryBlocks = 1500);
    static void setVolume(float vol);
};
```

### services/SynthDisplay.h
```cpp
#pragma once
#include <ILI9341_t3.h>
#include "core/HardwareCfg.h"

class SynthDisplay {
    static ILI9341_t3 tft;

public:
    static void init();
    static void print(const char* text);
    static void println(const char* text);
    static void print(int value);
    static void println(int value);
    static void clear();

    // Conditional logging (respects CONFIGURATION flags)
    static void logConnection(const char* from, const char* to);
    static void logDisconnection(const char* from, const char* to);
    static void logJackEvent(const char* name, bool connected);
    static void logModuleDetected(uint8_t type, const char* name, uint8_t slot);
};
```

### services/Modules.h
```cpp
#pragma once
#include <vector>

class Module;

class Modules {
public:
    static std::vector<Module*> active;

    static void updateAll();
};
```

---

## Comparison

| Aspect | Current | Static Services |
|--------|---------|-----------------|
| Find socket/cable state | 4 files | `Connections::` |
| Find audio setup | `Module::_audioCtrl` + `main.cpp` | `SynthAudio::` |
| Find display calls | `extern tft` everywhere | `SynthDisplay::` |
| Add new module | Copy scattered patterns | Clear service calls |

---

## Usage Example

### Before (current)
```cpp
// In InputSocket.cpp
void InputSocket::setAvailable(std::shared_ptr<InputSocket> i) {
    availableInputs.push_back(i);
    if(i->state == SocketState::BUSY)
        removeFromBusy(i);
    i->state = SocketState::AVAILABLE;
}

// In Module.h - Input struct constructor
inline Input(...) {
    base = std::make_shared<InputSocket>(...);
    Module::inputSockets.push_back(base);  // Side effect
}

// In PatchCable.cpp
void PatchCable::onInputSocketConnected(std::shared_ptr<InputSocket> i) {
    InputSocket::setAvailable(i);
    searchForCablesToAdd();
}
```

### After (static services)
```cpp
// In Connections.cpp
void Connections::onInputConnected(std::shared_ptr<InputSocket> socket) {
    // Move from busy to available if needed
    if (socket->state == SocketState::BUSY) {
        busyInputs.remove_if([&](auto& s) { return s->uid == socket->uid; });
    }
    availableInputs.push_back(socket);
    socket->state = SocketState::AVAILABLE;

    searchForCablesToAdd();
}

void Connections::registerInput(std::shared_ptr<InputSocket> socket) {
    inputs.push_back(socket);
}

// In Module.h - Input struct (no side effect)
inline Input(...) {
    socket = std::make_shared<InputSocket>(...);
    // Registration happens explicitly in module constructor
}

// In VCO_det.cpp
VCO_det::VCO_det(const Address& a) : Module(a), freqCv(...), sine(...) {
    Connections::registerInput(freqCv.socket);
    Connections::registerOutput(sine.socket);
    // Clear what's happening
}
```

---

## Refactored Module Base

```cpp
// core/Module.h
#pragma once
#include <vector>
#include <Audio.h>
#include "Address.h"

class Module {
protected:
    Address moduleAddress;
    std::vector<AudioConnection*> internalConns;

public:
    Module(const Address& addr) : moduleAddress(addr) {}
    virtual ~Module() = default;  // Added!

    virtual void updateValues() = 0;
};

// Input/Output stay as helpers but without side effects
struct Input {
    std::shared_ptr<InputSocket> socket;
    Input(const Address& a, uint8_t id, uint8_t detId,
          AudioStream& as, uint8_t port, const char* name = "in");
};

struct Output {
    std::shared_ptr<OutputSocket> socket;
    Output(const Address& a, uint8_t id, uint8_t detId,
           AudioStream& as, uint8_t port, const char* name = "out");
};
```

---

## Refactored main.cpp

```cpp
#include <Arduino.h>
#include "services/SynthAudio.h"
#include "services/SynthDisplay.h"
#include "services/Modules.h"
#include "services/Connections.h"
#include "core/ModuleFactory.h"
#include "core/HardwareCfg.h"

void setup() {
    // Pin setup
    initPins();

    // Services
    SynthAudio::init(1500);
    SynthDisplay::init();

    delay(2000);

    // Module detection
    ModuleFactory::detectAndCreate(Modules::active);
}

void loop() {
    Modules::updateAll();
    Connections::update();
}
```

---

## Migration Path

1. Create `services/` folder
2. Add `Connections.h/.cpp` - unify socket + cable state from InputSocket, OutputSocket, PatchCable
3. Add `SynthDisplay.h/.cpp` - wrap tft global
4. Add `SynthAudio.h/.cpp` - move from Module::_audioCtrl
5. Add `Modules.h/.cpp` - move from main.cpp
6. Update includes gradually
7. Remove old static members

Each step is small and testable.

---

## Benefits

- **Readability**: `Connections::registerInput(x)` - clear what it does
- **Discoverability**: All connection stuff in one place, not four files
- **No runtime cost**: Still static, still global, just organized
- **Explicit registration**: No hidden side effects in constructors
- **Easy migration**: One service at a time
- **Module authors**: Clear API - `Connections::register*`, `SynthDisplay::log*`
