#include "test_reader.h"
#include "../core/hardware_cfg.h"
#include <Arduino.h>

uint16_t TestReader::readAnalog(const Address& slot, uint8_t pinId) {
	ControlAddress addr(slot, pinId);
	addr.setForReading();

	// 5-sample average (same as Potentiometer class)
	uint32_t sum = 0;
	for (uint8_t i = 0; i < POT_READS; ++i) {
		sum += analogRead(READ_PIN);
	}

	return sum / POT_READS;
}

bool TestReader::readDigital(const Address& slot, uint8_t pinId, TestControlType type) {
	ControlAddress addr(slot, pinId);
	addr.setForReading();

	bool raw = digitalRead(READ_PIN);

	// BUTTON and JACK_DETECTOR are active LOW (inverted)
	// SWITCH reads directly
	if (type == TestControlType::BUTTON || type == TestControlType::JACK_DETECTOR) {
		return !raw;
	}

	return raw;
}
