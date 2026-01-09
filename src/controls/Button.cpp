#include "Button.h"

//ctor
Button :: Button
(const Address& slotAddress, uint_fast8_t id) 
: 
Control (slotAddress, id) 
{
	state = false;
}

bool Button::tap()
{
	if (wasUpdated() && state) {
		delay(BOUNCE_DELAY);
		if(!isPressed()) 
			return true;
	}
	return false;
}

bool Button::longPress()
{
	if (wasUpdated() && state) {
		int counter = LONG_PRESS_TIME;
		while (counter > 0) {
			if (!isPressed())
				return false;
			counter -= BOUNCE_DELAY;
		}
		return true;
	}
	return false;
}

bool
Button :: isPressed(){
	update();
	return state;
}

void
Button :: update(){
	address.setForReading();
	bool r = !digitalRead(address.getPin());
	if(r!= state){
		state = r;
		_wasUpdated=true;
	}
	else 
		_wasUpdated=false;
}