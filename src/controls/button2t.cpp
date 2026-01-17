#include "button2t.h"

//ctor
Button2t :: Button2t (const Address& slotAddress, uint_fast8_t modulePin) 
: Button (slotAddress, modulePin) 
{}

bool Button2t::tap()
{
	if (wasUpdated() && state) {
		delay(DOUBLE_TAP_DELAY);
		if (!isPressed()) {
			delay(DOUBLE_TAP_DELAY);
			if (isPressed()) {
				detectedTapCount++;
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
	bool temp = detectedTapCount > 0;
	detectedTapCount = false;
	return temp;
}
