#ifndef __MODULES_AUDIO_IN_H__
#define __MODULES_AUDIO_IN_H__

#include "../Module.h"

class AudioIn : public Module {
	private:
		AudioInputI2S _lineIn;
		AudioAmplifier _preamp;
		AudioInputUSB _usbIn;
		Potentiometer _micGain;
	public:
		AudioIn(const Address&);		
		void updateValues() override;
};

#endif