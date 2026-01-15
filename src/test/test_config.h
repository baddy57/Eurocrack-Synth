#pragma once
#include <cstdint>
#include <vector>

enum class TestControlType : uint8_t {
	POTENTIOMETER,
	BUTTON,
	SWITCH,
	JACK_DETECTOR,
	SELECTOR_MULTI
};

// Lightweight struct for test control info - no separate config files needed
struct TestControlInfo {
	const char* name;
	uint8_t pinId;
	TestControlType type;
};
