#ifndef __CONTROLS_POTENTIOMETER_H__
#define __CONTROLS_POTENTIOMETER_H__

#include "../Control.h"

#define DIV1023 0.0009775

#define LIN 0
#define EXP 1
#define LOG 2
#define POW 3

class Potentiometer : public Control {
	private:
		/*inherited
		ControlAddress address
		bool _wasUpdated; */
		float value = -10.f;
		float minRead = 0.f, maxRead = 1023.f;
		float rangeScope = 0;
		float rangeMin = 0.f;
		float rangeMax = 0.f;
		float range = 1.f;
		float k = 1;
		int scale = LIN;
		void update();
	public:

		/// @brief constructor
		/// @param a address of the module
		/// @param id pin on the module pcb
		/// @param pullup_res value of the pullup resistor
		Potentiometer(const Address& a, uint_fast8_t id, float pullup_res = 0.f) : Control(a, id), value(0) { setPullUpResistor(pullup_res); };
		
		/// @brief constructor for pots connected directly to the board
		/// @param pin 
		Potentiometer(uint_fast8_t pin) : Control(pin), value(0){};

		/// @brief set the value of the pullup to fix readings
		/// @param value of the pullup resistor in ohm 
		void setPullUpResistor(float);

		/// @brief set the target range of the reading
		/// @param min value
		/// @param max value
		/// @param flag LIN,LOG,EXP,POW
		void setRange(float, float, int flag = LIN);

		/// @brief reads the hardware value
		/// @return true if the value is different from the last read
		bool wasUpdated() {update(); return _wasUpdated;};

		/// @brief translates the hardware value into target value
		/// @return a value from the range set before
		float read();

		/// @brief 
		/// @return original hardware value {0,1023} 
		inline int getOriginalValue() { return value; }
};

typedef Potentiometer Fader;
#endif