#pragma once

#include "../core/Module.h"

class AudioIn : public Module {
	private:
		AudioInputI2S _lineIn;
		AudioAmplifier _preamp;
		AudioInputUSB _usbIn;
		Potentiometer _micGain;
		Output lineInL;
		Output lineInR;
		Output usbInL;
		Output usbInR;
	public:
		AudioIn(const Address&);		
		void updateValues() override;
};
