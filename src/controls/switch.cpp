#include "switch.h"

void
Switch :: update() {
	address.setForReading();

	bool r=digitalRead(address.getArduinoPin());

	if (r!=value) {
		_wasUpdated = true;
		value = r;
	}
	else 
		_wasUpdated = false;
}
