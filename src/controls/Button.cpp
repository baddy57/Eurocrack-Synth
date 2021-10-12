#include "Button.h"

#include <cstdint>
#include <Wire.h>

//ctor
Button :: Button
(const Address& slotAddress, uint_fast8_t id) 
: 
Control (slotAddress, id) 
{
	state = false;
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