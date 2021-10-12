#ifndef __MODULES_AUDIO_OUT_H__
#define __MODULES_AUDIO_OUT_H__ 1

#include "../Module.h"
#include "../Address.h"
#include <Audio.h>
#include <cstdint>
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

class AudioOut : public Module {
	private:
		AudioControlSGTL5000 _audioCtrl;
		AudioOutputI2S _analogOut;
		AudioOutputUSB _usbOut;
		Potentiometer _volume_pot0;
	public:
		AudioOut(const Address&);
		void updateValues() override;
		
		//void updateConnections();
};
#endif
