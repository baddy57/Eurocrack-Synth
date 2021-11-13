#include "Button2t.h"

//ctor
Button2t :: Button2t (const Address& slotAddress, uint_fast8_t id) 
: Button (slotAddress, id) 
{}

bool Button2t::tap()
{
	if (wasUpdated() && state) {
		delay(DOUBLE_TAP_DELAY);
		if (!isPressed()) {
			delay(DOUBLE_TAP_DELAY);
			if (isPressed()) {
				doubleTapDetected++;
				return false;
			}
			else return true;
		}
	}
	return false;
}

bool Button2t::doubleTap()
{
	tap();
	bool temp = doubleTapDetected;
	doubleTapDetected = false;
	return temp;
}
