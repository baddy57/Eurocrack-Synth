#pragma once

#include "../core/Module.h"

class AudioOut : public Module {
private:
		AudioOutputI2S _analogOut;
		AudioOutputUSB _usbOut;
		Potentiometer _volume_pot0;
		Input analogOutL;
		Input analogOutR;
		Input usbOutL;
		Input usbOutR;
	public:
		AudioOut(const Address&);
		void updateValues() override;
};