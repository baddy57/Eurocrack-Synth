#include "Potentiometer.h"

#include <Wire.h>
#include <cstdint>
#include <math.h>

extern const uint_fast8_t POT_DEADZONE;

extern ILI9341_t3 tft;
extern const uint_fast8_t POT_READS;


void Potentiometer::setRange(float pullup){
	if (pullup == 0.f)return;
	minval = pullup / 9.78f;
	range = 1023.f / (990.f-minval);
	return;
}

float Potentiometer::read(float min, float max, uint scaleType) {
	switch (scaleType) {
	case LIN:
		return value / 1023.f * (max - min) + min;
	case EXP:
		return pow(2, (uint)value / 8) / pow(2, 127) * (max - min) + min;
	case LOG:
		return 0;
	default:
		return -1;
	}

}

void
Potentiometer :: update() {
	address.setForReading();
	int r_sum =0;
	uint_fast8_t p = address.getPin();

	for (uint i = 0; i < POT_READS; ++i)
		r_sum +=analogRead(p);

	float avg = r_sum / POT_READS;
	avg -= minval;
	avg *= range;
	if(avg < 0.f) avg = 0.f;
	if (avg > 1023.f) avg = 1023.f;
	
	if (avg > (value+5.f)/*POT_DEADZONE*/ || avg < (value-5.f)/*POT_DEADZONE*/) {
		value = avg;
		_wasUpdated = true;
		return;
	}
	else //if the value is the same as before 
		_wasUpdated = false;
	return;
}