#pragma once

#include "../core/address.h"
#include "test_config.h"

class TestReader {
public:
	// Read raw analog value (0-1023), 5-sample average
	static uint16_t readAnalog(const Address& slot, uint8_t pinId);

	// Read digital input (returns true/false)
	// BUTTON and JACK_DETECTOR are inverted (active LOW)
	// SWITCH is direct read
	static bool readDigital(const Address& slot, uint8_t pinId, TestControlType type);
};
