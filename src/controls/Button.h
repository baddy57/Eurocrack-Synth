#pragma once

#define BOUNCE_DELAY 20
#define DOUBLE_TAP_DELAY 100
#define LONG_PRESS_TIME 500

#include "../core/Control.h"

class Button : public Control {
	protected:
		bool state;
		int detectedTapCount = 0;
	public:
		Button(const Address&, uint_fast8_t);
		virtual bool tap();
		bool longPress();
		void update();
		bool isPressed();
		bool wasUpdated() {update(); return _wasUpdated;};
};
