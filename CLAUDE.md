# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Eurocrack Synth - A software-based Eurorack modular synthesizer emulator running on Teensy 4.1. Audio is processed internally on the Teensy while physical modules act as controllers. No actual audio flows through patch cables.

## Build Commands

**Build:** `platformio run` or `pio run`
**Upload:** `platformio run --target upload` (uses teensy-gui)
**Clean:** `platformio run --target clean`
**Serial Monitor:** `platformio device monitor` (31250 baud - MIDI rate)
**Test:** `platformio test`

External libraries are expected in `../libraries/` relative to the project root.

## Architecture

### Core System (`src/core/`)

- **Module** - Base class for all synthesis modules. Manages audio connections via Teensy Audio Library, input/output socket collections, and update priority system.
- **ModuleFactory** - Detects modules via ID multiplexers at startup and instantiates appropriate module types. Supports up to 16/64 modules.
- **PatchCable** - Represents virtual connections between sockets. Created when physical jacks are connected, destroyed when unplugged. Supports 4-voice polyphony.
- **Address** - Hierarchical addressing system for multiplexed hardware (12-bit slot addresses, control addresses, output socket addresses).
- **HardwareCfg.h** - Pin assignments and hardware configuration. Supports MOBO_REV_3 and MOBO_REV_4 board revisions.

### Controls (`src/controls/`)

Control abstractions for hardware inputs: Potentiometer, Button, Button2t, Switch, RotarySwitch, InputSocket, OutputSocket. Sockets support mono/poly modes and three states (INACTIVE, AVAILABLE, BUSY).

### Services (`src/services/`)

Static service classes organizing global state. No interfaces, no DI, no runtime cost - just better organization.

- **SynthDisplay** - TFT display wrapper. Access via `SynthDisplay::print()`, `SynthDisplay::raw()` for direct ILI9341_t3 access.
- **SynthAudio** - Audio codec (`SynthAudio::codec`). Note: `AudioMemory()` must be called from main.cpp.
- **Modules** - Active module list (`Modules::active`), main loop update (`Modules::updateAll()`).
- **Connections** - Unified socket/cable management. Entry point for jack event polling (`Connections::update()`).

### Modules (`src/modules/`)

Synthesis modules built on Teensy Audio Library streams (header-only with inline methods):
- VCO_det, VCA, VCF, LFO, ADSR - Core synthesis
- Mixer8 - 8-channel mixing
- AudioIn/AudioOut - I/O
- MidiMono, DrumMachine - MIDI control
- Reverb, Distortion_amp, Distortion_bc, Delay_single, Delay_multi - Effects

### Module Type IDs (`ModuleTypeIds.h`)

```
0=Empty, 1=AudioOut, 2=AudioIn, 3=MIDI, 4=VCO, 5=LFO, 6=Filter,
7=ADSR, 8=VCA, 9=Mixer8, 10=Distortion, 11=Reverb, 12=Delay,
13=Looper, 14=Noise, 15=Flanger, 17=DrumMachine, 18=Instruments,
19=DelaySingle, 255=EmptyModule
```

## Hardware Addressing

Six levels of cascaded 4051/4053 multiplexers:
- Mobo Level 1: selectors A,B,C
- Mobo Level 2: selectors D,E,F
- Rack Level 1: selectors G,H,I
- Rack Level 2: selectors J,K,L
- Module Level 1: selectors M,N,O
- Module Level 2: selectors P,Q,R (read only, big modules)

Each slot has a 12-bit address (selectors A-L). Modules have 24/48 control pins and 8 output socket pins.

## Polyphony

4-voice polyphony throughout. Connection rules:
- mono->mono: direct
- poly->poly: direct
- poly->mono: only voice[0] connected
- mono->poly: connected to voice[0]

## Lifecycle

Modules instantiated at startup and never destroyed. Patch cables created/destroyed dynamically based on physical jack connections.
