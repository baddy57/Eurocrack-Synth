#include "Potentiometer.h"

#include <Wire.h>
#include <cstdint>

extern const uint_fast8_t POT_DEADZONE;
#define DIV1023 1/1023

extern ILI9341_t3 tft;
extern const uint_fast8_t POT_READS;

void Potentiometer::setRange(float pullup){
	if (pullup == 0)return;
	minval = pullup / 9.78f;
	range = 1023.f / (990.f-minval);
	return;
}

void
Potentiometer :: update() {
	address.setForReading();
	unsigned int r_sum =0;
	uint_fast8_t p = address.getPin();

	for (uint i = 0; i < POT_READS; ++i)
		r_sum +=analogRead(p);

	float avg = r_sum / POT_READS;
	avg -= minval;
	avg *= range;

	if (avg > (value+4)/*POT_DEADZONE*/ || avg < (value-4)/*POT_DEADZONE*/) {
		_wasUpdated = true;
		if (avg < 0.f) {
			value = 0;
			return;
		}
		if (avg > 1023.f) {
			value = 1023;
			return;
		}
		value = avg;
		return;
	}
	else //if the value is the same as before 
		_wasUpdated = false;
	return;
}