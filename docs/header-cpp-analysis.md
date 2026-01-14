# Header/CPP Separation Analysis

Analysis of when h/cpp separation is beneficial vs when header-only is viable.

## Core (services, base classes, controls)

**Files:** `Module.h`, `Address.h`, `InputSocket.h`, `OutputSocket.h`, proposed services

**Include pattern:** Included by many files (Module.h → 15+ modules, services → everywhere)

### Benefit of h/cpp separation

| Change to | Files recompiled |
|-----------|------------------|
| `InputSocket.cpp` | 1 file |
| `InputSocket.h` | InputSocket.cpp + every module that includes it (~15 files) |

**Verdict: Keep separation.** Implementation changes (bug fixes, tweaks) don't cascade to all dependents.

### What goes where

```cpp
// InputSocket.h - interface only
class InputSocket : public Socket {
protected:
    ControlAddress* address;
public:
    InputSocket(const Address&, uint8_t, uint8_t, AudioStream&, uint8_t, const char*);
    bool isReceiving() const;
};

// InputSocket.cpp - all implementation
InputSocket::InputSocket(...) { ... }
bool InputSocket::isReceiving() const { ... }
```

---

## Modules (VCO_det, ADSR, Looper, etc.)

**Include pattern:** Each module is included exactly once, by `ModuleFactory.h`

### The include chain

```
main.cpp
  └── ModuleFactory.h
        ├── VCO_det.h
        ├── ADSR.h
        ├── Looper.h
        └── ... (all modules)
```

### Benefit analysis

| Change to | What recompiles |
|-----------|-----------------|
| `VCO_det.cpp` | VCO_det.cpp only |
| `VCO_det.h` | VCO_det.cpp + main.cpp (via ModuleFactory.h) |

There IS a benefit: .cpp-only changes (parameter tweaks, bug fixes) compile faster.

**But how often is each case?**

| Change type | Touches | Frequency |
|-------------|---------|-----------|
| Tweak pot range | .cpp only | Common |
| Fix logic bug | .cpp only | Common |
| Add new pot/socket | Both .h and .cpp | Occasional |
| Add new output | Both | Rare |

Maybe 60-70% of module changes are .cpp-only.

### The navigation cost

With separation, adding a potentiometer requires:

```cpp
// VCO_det.h - add declaration
class VCO_det : public Module {
    Potentiometer _coarse_pot0;  // ← add here
    Potentiometer _newpot;       // ← add here
};

// VCO_det.cpp - add to constructor init list AND usage
VCO_det::VCO_det(const Address& a)
    : Module(a)
    , _coarse_pot0(a, 8, 3800.f)
    , _newpot(a, 16, 4700.f)     // ← add here
{
    _newpot.setRange(0, 100);    // ← add here
}

void VCO_det::updateValues() {
    if (_newpot.wasUpdated()) {  // ← add here
        // ...
    }
}
```

That's 3 places across 2 files. In C#-style (header-only), it's 2 places in 1 file.

### Header-only alternative for modules

```cpp
// VCO_det.h - everything in one file
#pragma once
#include "core/Module.h"

class VCO_det : public Module {
    Potentiometer _coarse_pot0;
    Potentiometer _fine_pot1;
    // ... all members

public:
    inline VCO_det(const Address& a)
        : Module(a)
        , _coarse_pot0(a, 8, 3800.f)
        , _fine_pot1(a, 10, 3200.f)
    {
        _coarse_pot0.setRange(0, 3200, EXP);
        // ... all init
    }

    inline void updateValues() override {
        if (_coarse_pot0.wasUpdated()) {
            // ... all logic
        }
    }
};
```

**Pros:**
- Single file per module (C#-like)
- Add pot = 2 locations in 1 file
- Easier to see whole module at once

**Cons:**
- Any change recompiles main.cpp too
- Slightly larger binary (inline expansion) - usually negligible with LTO
- All includes in header (but modules already do this)

---

## Recommendation

### Core: Keep h/cpp separation
- Widely included
- Separation gives real compile-time benefit
- Interface stability matters

### Modules: Header-only is viable

Given:
- Single developer
- C# background (unified files feel natural)
- Each module is self-contained
- Compile times on Teensy aren't terrible
- Navigation simplicity has daily value

The compile-time cost of header-only modules is small (maybe +2-3 seconds per change), but the navigation simplicity is a daily benefit.

### Suggested structure

```
src/
├── core/           # h/cpp separation
│   ├── Module.h
│   ├── Module.cpp
│   ├── Address.h
│   ├── Address.cpp
│   └── ...
├── services/       # h/cpp separation
│   ├── Connections.h
│   ├── Connections.cpp
│   └── ...
├── controls/       # h/cpp separation (shared by modules)
│   ├── Potentiometer.h
│   ├── Potentiometer.cpp
│   └── ...
└── modules/        # header-only (each included once)
    ├── VCO_det.h      # contains full implementation
    ├── ADSR.h
    ├── Looper.h
    └── ...
```

This gives C#-style single-file modules while keeping separation where it actually matters.
