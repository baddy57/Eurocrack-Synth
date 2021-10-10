#include "CONTROLS_Potentiometer.h"

#include "Control.h"
#include "HardwareCfg.h"
#include "Address.h"

#include <Wire.h>
#include <cstdint>

extern const uint_fast8_t POT_DEADZONE;
#define DIV1023 1/1023

extern ILI9341_t3 tft;
extern const uint_fast8_t POT_READS;

void
Potentiometer :: update() {
//	tft.println("Potentiometer :: update()");
	address.setForReading();
	int r_sum =0;
	uint_fast8_t p = address.getPin();
	for(int i=0; i<POT_READS; ++i)
		r_sum +=analogRead(p);
	int avg = r_sum / POT_READS;
	if (avg > (value+3)/*POT_DEADZONE*/ || avg < (value-3)/*POT_DEADZONE*/) {
		// tft.print(avg);
		// tft.print(" != ");
		// tft.println(value);
		//tft.print(" +- ");
		//tft.println(POT_DEADZONE);
		_wasUpdated = true;
		value = avg;
		return;
	}
	else //if the value is the same as before 
		_wasUpdated = false;
	return;
}