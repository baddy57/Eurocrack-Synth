#ifndef __CONTROLS_BUTTON_H__
#define __CONTROLS_BUTTON_H__ 1

#include "Control.h"
#include "Address.h"
#include <cstdint>

class Button : public Control {
	private:
		bool state;
	public:
		Button(const Address&, uint_fast8_t);
		void update();
		bool isPressed();
		bool wasUpdated() {update(); return _wasUpdated;};
};

#endif