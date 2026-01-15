#include "Switch.h"

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
