#ifndef __CONTROLS_SWITCH_H__
#define __CONTROLS_SWITCH_H__

#include "../Control.h"
#include "../Address.h"
#include <cstdint>

class Switch : public Control {
	private:
		bool value;
	public:
		Switch(const Address& a, uint_fast8_t id) : Control(a, id), value(0){};
		bool b_read() /* override */ {return value;};
		void update() /* override */;
		bool wasUpdated() {update(); return _wasUpdated;};
};

#endif