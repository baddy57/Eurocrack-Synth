# Test Mode Design Document

## Overview

A hardware debugging mode for the Eurocrack Synth that displays real-time control values on the TFT screen, enabling detection of bad solder joints, verification of potentiometer values, and testing of digital inputs without requiring external tools (except multimeter for output socket verification).

## Goals

1. **Hardware Verification**: Detect faulty solder joints, broken traces, misconfigured multiplexers
2. **Real-Time Monitoring**: Display live values that update in-place (no scrolling)
3. **Comprehensive Coverage**: Test potentiometers, buttons, switches, and jack detectors
4. **Minimal Intrusion**: Separate mode that doesn't affect normal operation

---

## Architecture

### Activation Method

Option A: **Compile-Time Flag**
```cpp
#define TEST_MODE_ENABLED 1
```

Option B: **Runtime Detection** (Recommended)
- Hold a specific button combination during boot
- Or detect a jumper/switch on the motherboard
- Or check if only slot 0 has a module (single-module test scenario)

### Mode Structure

```
┌─────────────────────────────────────────┐
│           TEST MODE ENTRY               │
│  (detected at boot or via flag)         │
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│         MODULE DETECTION                │
│  Scan slots, identify connected module  │
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│       LOAD MODULE TEST CONFIG           │
│  Get control definitions for module type│
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│         MAIN TEST LOOP                  │
│  Poll controls, update display in-place │
└─────────────────────────────────────────┘
```

---

## Data Structures

### Module Test Configuration

Since controls are private members and not enumerable at runtime, we define test configurations per module type:

```cpp
// src/test/test_config.h

enum class ControlType : uint8_t {
    POTENTIOMETER,
    BUTTON,
    SWITCH,
    ROTARY_SWITCH,
    SELECTOR_MULTI,
    JACK_DETECTOR
};

struct TestControl {
    const char* name;           // Display name (max 8 chars for grid)
    uint8_t controlId;          // ID within module (0-63)
    ControlType type;           // Control type
    uint8_t detectorId;         // For sockets: jack detector ID
    float pullupRes;            // For pots: pullup resistor value (0 if none)
};

struct ModuleTestConfig {
    uint8_t moduleTypeId;       // From ModuleTypeIds
    const char* moduleName;     // Display name
    const TestControl* analogControls;
    uint8_t numAnalogControls;
    const TestControl* digitalControls;
    uint8_t numDigitalControls;
};
```

### Example Configuration (VCO_det)

```cpp
// src/test/configs/vco_det_test.cpp

static const TestControl vco_det_analog[] = {
    {"COARSE",   8,  ControlType::POTENTIOMETER, 0, 3800.f},
    {"FINE",    10,  ControlType::POTENTIOMETER, 0, 3200.f},
    {"FM_CV",   11,  ControlType::POTENTIOMETER, 0, 0.f},
    {"PM_CV",   12,  ControlType::POTENTIOMETER, 0, 0.f},
    {"AMP",     15,  ControlType::POTENTIOMETER, 0, 0.f},
};

static const TestControl vco_det_digital[] = {
    {"FM/PM",    9,  ControlType::SWITCH,        0, 0.f},
    {"FREQ_JK", 13,  ControlType::JACK_DETECTOR, 24, 0.f},
    {"SHAP_JK", 14,  ControlType::JACK_DETECTOR, 29, 0.f},
    {"SINE_JK", 25,  ControlType::JACK_DETECTOR, 25, 0.f},
    {"TRI_JK",  26,  ControlType::JACK_DETECTOR, 26, 0.f},
    {"SAW_JK",  27,  ControlType::JACK_DETECTOR, 27, 0.f},
    {"SQR_JK",  28,  ControlType::JACK_DETECTOR, 28, 0.f},
};

const ModuleTestConfig VCO_DET_CONFIG = {
    .moduleTypeId = ModuleTypeIds::VCO_ID,
    .moduleName = "VCO_det",
    .analogControls = vco_det_analog,
    .numAnalogControls = 5,
    .digitalControls = vco_det_digital,
    .numDigitalControls = 7,
};
```

### Test Control Reader

```cpp
// src/test/test_reader.h

class TestControlReader {
public:
    // Read raw ADC value for a potentiometer
    static uint16_t readAnalogRaw(const Address& slot, uint8_t controlId);

    // Read digital input (button, switch, jack detector)
    static bool readDigital(const Address& slot, uint8_t controlId);

    // Read jack detector specifically (uses different address)
    static bool readJackDetector(const Address& slot, uint8_t detectorId);

private:
    static void setMuxAddress(const Address& slot, uint8_t controlId);
};
```

---

## Screen Layout

### Display Specifications
- Resolution: 320 x 240 pixels
- Text size 1: ~6x8 pixels per character (~53 chars x 30 lines)
- Text size 2: ~12x16 pixels per character (~26 chars x 15 lines)

### Layout Design

```
┌──────────────────────────────────────────────────┐  Y=0
│  TEST MODE - VCO_det                             │  Header (16px)
│  Slot: 0  Type ID: 4  Address: 0x00              │  Info (16px)
├──────────────────────────────────────────────────┤  Y=32
│  ANALOG CONTROLS                                 │  Section (12px)
├──────────────────────────────────────────────────┤  Y=44
│  NAME     PIN   RAW    VALUE                     │  Column headers
│  ──────────────────────────────────────────────  │
│  COARSE    8    512    8192 Hz                   │  Row 1
│  FINE     10    768    440 Hz                    │  Row 2
│  FM_CV    11    1023   1.00                      │  Row 3
│  PM_CV    12    0      0.00                      │  Row 4
│  AMP      15    512    0.50                      │  Row 5
├──────────────────────────────────────────────────┤  Y=140
│  DIGITAL INPUTS                                  │  Section
├──────────────────────────────────────────────────┤
│  NAME     PIN   STATE     NAME     PIN   STATE   │  2-column grid
│  ──────────────────────────────────────────────  │
│  FM/PM     9    [ON ]     FREQ_JK  24   [----]   │
│  SINE_JK  25    [JACK]    TRI_JK   26   [----]   │
│  SAW_JK   27    [----]    SQR_JK   28   [JACK]   │
│  SHAP_JK  29    [----]                           │
└──────────────────────────────────────────────────┘  Y=240
```

### Color Scheme

```cpp
#define COLOR_BACKGROUND    ILI9341_BLACK   // 0x0000
#define COLOR_HEADER        ILI9341_CYAN    // 0x07FF
#define COLOR_LABEL         ILI9341_WHITE   // 0xFFFF
#define COLOR_VALUE_NORMAL  ILI9341_GREEN   // 0x07E0
#define COLOR_VALUE_ZERO    ILI9341_YELLOW  // 0xFFE0
#define COLOR_VALUE_MAX     ILI9341_RED     // 0xF800
#define COLOR_DIGITAL_ON    ILI9341_GREEN   // 0x07E0
#define COLOR_DIGITAL_OFF   0x4208          // Dark gray
#define COLOR_JACK_PRESENT  ILI9341_MAGENTA // 0xF81F
#define COLOR_SEPARATOR     0x4208          // Dark gray
```

---

## Implementation

### File Structure

```
src/
├── test/
│   ├── test_mode.h              # Main test mode class
│   ├── test_mode.cpp
│   ├── test_config.h            # Data structures
│   ├── test_reader.h            # Hardware reading utilities
│   ├── test_reader.cpp
│   ├── test_display.h           # Display rendering
│   ├── test_display.cpp
│   └── configs/                 # Module-specific configs
│       ├── vco_det_test.cpp
│       ├── mixer8_test.cpp
│       ├── adsr_test.cpp
│       └── ...
```

### Core Classes

#### TestMode (Main Controller)

```cpp
// src/test/test_mode.h

class TestMode {
public:
    static void enter();              // Enter test mode
    static void exit();               // Exit to normal mode
    static void update();             // Main loop update
    static bool isActive();           // Check if in test mode

private:
    static bool _active;
    static Address _currentSlot;
    static const ModuleTestConfig* _currentConfig;

    static void detectModule();
    static void drawStaticUI();
    static void updateAnalogValues();
    static void updateDigitalValues();
};
```

#### TestReader (Hardware Interface)

```cpp
// src/test/test_reader.cpp

uint16_t TestControlReader::readAnalogRaw(const Address& slot, uint8_t controlId) {
    ControlAddress addr(slot, controlId);
    addr.setForReading();

    // Multi-sample for stability (like Potentiometer does)
    uint32_t sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += analogRead(READ_PIN);
    }
    return sum / 5;
}

bool TestControlReader::readDigital(const Address& slot, uint8_t controlId) {
    ControlAddress addr(slot, controlId);
    addr.setForReading();
    delayMicroseconds(MUX_DELAY);
    return digitalRead(READ_PIN);
}

bool TestControlReader::readJackDetector(const Address& slot, uint8_t detectorId) {
    // Jack detectors use a different addressing scheme
    // Based on Socket::hasJack() implementation
    ControlAddress addr(slot, detectorId);
    addr.setForReading();
    delayMicroseconds(MUX_DELAY);
    return !digitalRead(READ_PIN);  // Active low
}
```

#### TestDisplay (Screen Rendering)

```cpp
// src/test/test_display.h

class TestDisplay {
public:
    static void init();
    static void drawHeader(const char* moduleName, uint8_t slot, uint8_t typeId);
    static void drawAnalogSection(uint8_t startY);
    static void drawDigitalSection(uint8_t startY);

    // In-place value updates (no full redraw)
    static void updateAnalogValue(uint8_t row, uint16_t raw, float processed);
    static void updateDigitalState(uint8_t row, uint8_t col, bool state, bool isJack);

private:
    static void clearValueArea(uint8_t x, uint8_t y, uint8_t width);

    // Screen coordinates
    static constexpr uint8_t HEADER_Y = 0;
    static constexpr uint8_t INFO_Y = 16;
    static constexpr uint8_t ANALOG_SECTION_Y = 32;
    static constexpr uint8_t ANALOG_START_Y = 56;
    static constexpr uint8_t DIGITAL_SECTION_Y = 140;
    static constexpr uint8_t DIGITAL_START_Y = 164;

    // Column positions
    static constexpr uint8_t COL_NAME = 4;
    static constexpr uint8_t COL_PIN = 70;
    static constexpr uint8_t COL_RAW = 110;
    static constexpr uint8_t COL_VALUE = 170;
};
```

### Main Loop Integration

```cpp
// src/main.cpp modifications

#include "test/test_mode.h"

void setup() {
    // ... existing setup code ...

    #if TEST_MODE_ENABLED
    // Check for test mode activation (e.g., button held during boot)
    if (shouldEnterTestMode()) {
        TestMode::enter();
        return;  // Skip normal initialization
    }
    #endif

    // ... normal module factory, etc ...
}

void loop() {
    #if TEST_MODE_ENABLED
    if (TestMode::isActive()) {
        TestMode::update();
        return;
    }
    #endif

    // ... normal loop ...
}
```

---

## Value Display Format

### Analog Controls

| Control Type | Raw Display | Processed Display |
|--------------|-------------|-------------------|
| Potentiometer (freq) | `0-1023` | `20-20000 Hz` |
| Potentiometer (gain) | `0-1023` | `0.00-1.00` |
| Potentiometer (time) | `0-1023` | `0-2000 ms` |
| SelectorMulti | `0-1023` | `Zone 1/4` |

### Digital Controls

| Control Type | State Display |
|--------------|---------------|
| Button (released) | `[----]` (gray) |
| Button (pressed) | `[PUSH]` (green) |
| Switch (off) | `[OFF ]` (gray) |
| Switch (on) | `[ ON ]` (green) |
| Jack (no cable) | `[----]` (gray) |
| Jack (cable inserted) | `[JACK]` (magenta) |

---

## Update Strategy

### In-Place Updates

To avoid flickering and scrolling, values are updated in-place:

```cpp
void TestDisplay::updateAnalogValue(uint8_t row, uint16_t raw, float processed) {
    uint8_t y = ANALOG_START_Y + (row * 12);

    // Clear only the value areas
    tft.fillRect(COL_RAW, y, 50, 10, COLOR_BACKGROUND);
    tft.fillRect(COL_VALUE, y, 80, 10, COLOR_BACKGROUND);

    // Draw new values
    tft.setCursor(COL_RAW, y);
    tft.setTextColor(getAnalogColor(raw));
    tft.printf("%4d", raw);

    tft.setCursor(COL_VALUE, y);
    tft.printf("%.2f", processed);
}

uint16_t TestDisplay::getAnalogColor(uint16_t raw) {
    if (raw < 10) return COLOR_VALUE_ZERO;
    if (raw > 1013) return COLOR_VALUE_MAX;
    return COLOR_VALUE_NORMAL;
}
```

### Update Rate

```cpp
// Throttle updates to avoid overwhelming the display
static constexpr uint32_t UPDATE_INTERVAL_MS = 50;  // 20 Hz refresh

void TestMode::update() {
    static uint32_t lastUpdate = 0;
    uint32_t now = millis();

    if (now - lastUpdate < UPDATE_INTERVAL_MS) return;
    lastUpdate = now;

    updateAnalogValues();
    updateDigitalValues();
}
```

---

## Module Configuration Registry

```cpp
// src/test/test_configs.cpp

#include "configs/vco_det_test.cpp"
#include "configs/mixer8_test.cpp"
#include "configs/adsr_test.cpp"
// ... more configs ...

static const ModuleTestConfig* ALL_CONFIGS[] = {
    &VCO_DET_CONFIG,
    &MIXER8_CONFIG,
    &ADSR_CONFIG,
    &VCA_CONFIG,
    &VCF_CONFIG,
    &LFO_CONFIG,
    &REVERB_CONFIG,
    &DELAY_SINGLE_CONFIG,
    &DELAY_MULTI_CONFIG,
    &DISTORTION_AMP_CONFIG,
    &DISTORTION_BC_CONFIG,
    &AUDIO_IN_CONFIG,
    &AUDIO_OUT_CONFIG,
    &MIDI_MONO_CONFIG,
    &DRUM_MACHINE_CONFIG,
    &LOOPER_CONFIG,
    nullptr  // Terminator
};

const ModuleTestConfig* findConfigByTypeId(uint8_t typeId) {
    for (int i = 0; ALL_CONFIGS[i] != nullptr; i++) {
        if (ALL_CONFIGS[i]->moduleTypeId == typeId) {
            return ALL_CONFIGS[i];
        }
    }
    return nullptr;
}
```

---

## Error Handling

### No Module Detected

```cpp
void TestMode::detectModule() {
    for (uint8_t slot = 0; slot < MAX_MODULES; slot++) {
        Address addr(slot);
        uint8_t typeId = ModuleTypeIdMux(addr).getModuleId();

        if (typeId != 0 && typeId != 255) {
            _currentSlot = addr;
            _currentConfig = findConfigByTypeId(typeId);

            if (_currentConfig == nullptr) {
                displayError("Unknown module type: " + String(typeId));
            }
            return;
        }
    }

    displayError("No module detected!");
}
```

### Display Error Screen

```cpp
void TestDisplay::showError(const char* message) {
    tft.fillScreen(COLOR_BACKGROUND);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(20, 100);
    tft.println("ERROR");
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(20, 130);
    tft.println(message);
}
```

---

## Future Enhancements

1. **Multi-Module Support**: Navigate between slots with buttons
2. **Value Logging**: Record values over time to Serial/SD card
3. **Automated Testing**: Run predefined test sequences
4. **Calibration Mode**: Adjust pot ranges and save to EEPROM
5. **Waveform Preview**: Show output waveform on display (if DAC available)
6. **Network Debug**: Stream values via USB/Ethernet for PC visualization

---

## Implementation Checklist

- [ ] Create `src/test/` directory structure
- [ ] Implement `TestControlReader` class
- [ ] Implement `TestDisplay` class
- [ ] Create module test configurations (start with VCO_det)
- [ ] Implement `TestMode` main controller
- [ ] Add test mode entry detection to `main.cpp`
- [ ] Add build flag `TEST_MODE_ENABLED`
- [ ] Test with physical hardware
- [ ] Create remaining module configurations
- [ ] Add documentation to CLAUDE.md
