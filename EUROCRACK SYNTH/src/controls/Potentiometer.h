#ifndef __CONTROLS_POTENTIOMETER_H__
#define __CONTROLS_POTENTIOMETER_H__

#include "../Control.h"

#define DIV1023 0.0009775

#define LIN 0
#define EXP 1
#define LOG 2

class Potentiometer : public Control {
	private:
		/*inherited
		ControlAddress address
		bool _wasUpdated; */
		float value = -10.f;
		float minval = 0.f;// , maxval = 1023;
		float range = 1.f;
	public:
		Potentiometer(const Address& a, uint_fast8_t id, float pullup_res = 0.f) : Control(a, id), value(0) { setRange(pullup_res); };
		Potentiometer(uint_fast8_t pin) : Control(pin), value(0){};
		void setRange(float);
		float read(float, float, uint = LIN);
		float f_read(){return (value*DIV1023);};
		float half_f_read(){return ((value-200)*DIV1023);};
		float i_read(){return value;};
		void update();
		bool wasUpdated() {update(); return _wasUpdated;};
};

typedef Potentiometer Fader;
#endif