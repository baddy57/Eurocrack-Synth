#include "Switch.h"

#include <Wire.h>
#include <cstdint>
extern ILI9341_t3 tft;

void
Switch :: update() {
	address.setForReading();
	bool r=digitalRead(address.getPin());
	/*
	bool r;
	uint_fast8_t r0=0,r1=0;
	for(uint_fast8_t i=0; i<5; ++i){
		if(digitalRead(address.getPin()))
			++r0;
		else ++r1;
	}
	if(r0>r1) r=0;
	else r=1;*/
	if (r!=value) {
		_wasUpdated = true;
	//	tft.println("sw updated");
	//	tft.print("v=");
	//	tft.print(value);
	//	tft.print(" r=");
	//	tft.print(r);
		value = r;
	//	tft.print(" new v=");
	//	tft.println(value);
	}
	else _wasUpdated = false;
	return;
}
