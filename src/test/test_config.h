#pragma once
#include <cstdint>

enum class TestControlType : uint8_t {
	POTENTIOMETER,
	BUTTON,
	SWITCH,
	JACK_DETECTOR,
	SELECTOR_MULTI  // Multi-position selector (reads as analog)
};

struct TestControl {
	const char* name;        // Max 8 chars for display grid
	uint8_t pinId;           // Control ID within module (0-63)
	TestControlType type;
	float pullupRes;         // For pots: pullup resistor (0 if none)
};

struct ModuleTestConfig {
	uint8_t moduleTypeId;
	const char* moduleName;
	const TestControl* analogControls;
	uint8_t numAnalog;
	const TestControl* digitalControls;
	uint8_t numDigital;
};

// Registry function
const ModuleTestConfig* findTestConfig(uint8_t typeId);
