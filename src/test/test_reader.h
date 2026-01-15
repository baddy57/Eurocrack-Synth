#pragma once

#include "../core/address.h"
#include "../core/hardware_cfg.h"
#include "test_config.h"
#include <Arduino.h>

class TestReader {
public:
	// Read raw analog value (0-1023), 5-sample average
	static inline uint16_t readAnalog(const Address& slot, uint8_t pinId) {
		ControlAddress addr(slot, pinId);
		addr.setForReading();

		// 5-sample average (same as Potentiometer class)
		uint32_t sum = 0;
		for (uint8_t i = 0; i < POT_READS; ++i) {
			sum += analogRead(READ_PIN);
		}

		return sum / POT_READS;
	}

	// Read digital input (returns true/false)
	// BUTTON and JACK_DETECTOR are inverted (active LOW)
	// SWITCH is direct read
	static inline bool readDigital(const Address& slot, uint8_t pinId, TestControlType type) {
		ControlAddress addr(slot, pinId);
		addr.setForReading();

		bool raw = digitalRead(READ_PIN);

		// BUTTON is active LOW (inverted)
		// SWITCH and JACK_DETECTOR read directly (jack detector inversion handled in display)
		if (type == TestControlType::BUTTON) {
			return !raw;
		}

		return raw;
	}
};
