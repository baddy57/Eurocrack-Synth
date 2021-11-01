#ifndef __CONTROLS_POTENTIOMETER_H__
#define __CONTROLS_POTENTIOMETER_H__

#include "../Control.h"
#include "../Address.h"
#include <cstdint>

#define DIV1023 1/1023

class Potentiometer : public Control {
	private:
		/*inherited
		ControlAddress address
		bool _wasUpdated; */
		unsigned int value;
		float minval = 0;// , maxval = 1023;
		float range = 1;
	public:
		Potentiometer(const Address& a, uint_fast8_t id, uint pullup_res = 0) : Control(a, id), value(0) { setRange(pullup_res); };
		Potentiometer(uint_fast8_t pin) : Control(pin), value(0){};
		void setRange(float);
		float f_read(){return (((float)value)*DIV1023);};
		float half_f_read(){return (((float)value-200)*DIV1023);};
		unsigned int i_read(){return value;};
		void update();
		bool wasUpdated() {update(); return _wasUpdated;};
};

#endif