#include "Potentiometer.h"

#include <math.h>

extern const float POT_DEADZONE;

extern ILI9341_t3 tft;
extern const uint_fast8_t POT_READS;


void Potentiometer::setPullUpResistor(float pullup){
	if (pullup == 0.f)return;
	minRead = pullup / 9.78f;
	range = 1023.f / (990.f-minRead);
	return;
}

void Potentiometer::setRange(float _min, float _max, int flag)
{
	scale = flag;
	rangeMin = _min;
	rangeMax = _max;
	rangeScope = _max - _min;

	switch (scale) {
	case LIN:
		k = (_max - _min) / 1023.f;
		break;
	case EXP:
		k = 1 / 1023.f;
		break;
	}
}

float Potentiometer::read() {
	switch (scale) {
	case LIN:
		return value * k + rangeMin;// / 1023.f * (max - rangeMin) + rangeMin;
	case EXP:
		return (exp(value * k)-1)*rangeScope / (M_E-1) + rangeMin;
	case LOG:
		return 0;
	case POW:
		return 0;// pow(2, (uint)value / 8) / pow(2, 127) * (max - rangeMin) + rangeMin;
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
	avg -= minRead;
	avg *= range;
	if(avg < 2.f) avg = 0.f;
	if (avg > 1020.f) avg = 1023.f;
	
	if (avg > value+POT_DEADZONE || avg < value-POT_DEADZONE) {
		value = avg;
		_wasUpdated = true;
		return;
	}
	else //if the value is the same as before 
		_wasUpdated = false;
	return;
}