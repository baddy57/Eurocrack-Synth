#include "Switch.h"

#include <Wire.h>
#include <cstdint>
extern ILI9341_t3 tft;

void
Switch :: update() {
	address.setForReading();
	bool r=digitalRead(address.getPin());
	if (r!=value) {
		_wasUpdated = true;
		value = r;
	}
	else _wasUpdated = false;
	return;
}
