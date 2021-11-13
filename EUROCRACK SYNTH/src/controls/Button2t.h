#ifndef __CONTROLS_BUTTON2T_H__
#define __CONTROLS_BUTTON2T_H__

#include "Button.h"

class Button2t : public Button {
	public:
		Button2t(const Address&, uint_fast8_t);
		bool tap() override;
		bool doubleTap();
};

#endif