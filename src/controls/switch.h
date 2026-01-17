#pragma once

#include "../core/Control.h"

class Switch : public Control {
	private:
		bool value;
	public:
		Switch(const Address& a, uint_fast8_t modulePin) : Control(a, modulePin), value(0){};
		bool b_read() /* override */ {return value;};
		void update() /* override */;
		bool wasUpdated() {update(); return _wasUpdated;};
};
