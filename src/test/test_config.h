#pragma once
#include <cstdint>
#include <vector>

class Potentiometer;  // Forward declaration

enum class TestControlType : uint8_t {
	POTENTIOMETER,
	BUTTON,
	SWITCH,
	JACK_DETECTOR,
	SELECTOR_MULTI
};

// Struct for analog test control info - includes pointer to control for computed value
struct TestControlInfo {
	const char* name;
	uint8_t pinId;
	TestControlType type;
	Potentiometer* pot;  // Pointer to potentiometer for reading computed value (nullptr for digital)

	static TestControlInfo createAnalog(const char* n, uint8_t p, Potentiometer* ctrl) {
		return {n, p, TestControlType::POTENTIOMETER, ctrl};
	}

	static TestControlInfo createDigital(const char* n, uint8_t p, TestControlType t) {
		return {n, p, t, nullptr};
	}
};
