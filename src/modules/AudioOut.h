#pragma once

#include "../core/Module.h"
#include "../services/SynthAudio.h"

namespace AudioOut_pins {
	enum inputs { ANALOG_OUT_L_D=16, ANALOG_OUT_L, 
				  ANALOG_OUT_R_D, ANALOG_OUT_R, 
				  USB_OUT_L_D, USB_OUT_L, 
				  USB_OUT_R_D, USB_OUT_R=23 };
}

class AudioOut : public Module {
private:
	AudioOutputI2S _analogOut;
	AudioOutputUSB _usbOut;
	Potentiometer _volume_pot0;
	ModuleInput analogOutL;
	ModuleInput analogOutR;
	ModuleInput usbOutL;
	ModuleInput usbOutR;

public:
	inline AudioOut(const Address& a)
		: Module(a)
		, _volume_pot0(pins::VOLUME)
		, analogOutL(a, AudioOut_pins::ANALOG_OUT_L, AudioOut_pins::ANALOG_OUT_L_D, _analogOut, 0, "ANALOG_OUT_L")
		, analogOutR(a, AudioOut_pins::ANALOG_OUT_R, AudioOut_pins::ANALOG_OUT_R_D, _analogOut, 1, "ANALOG_OUT_R")
		, usbOutL(a, AudioOut_pins::USB_OUT_L, AudioOut_pins::USB_OUT_L_D, _usbOut, 0, "USB_OUT_L")
		, usbOutR(a, AudioOut_pins::USB_OUT_R, AudioOut_pins::USB_OUT_R_D, _usbOut, 1, "USB_OUT_R")
	{
		SynthAudio::codec.enable();
		SynthAudio::codec.volume(0.5);
		_volume_pot0.setRange(0, 0.8, EXP);
	}

	inline void updateValues() override {
		if (_volume_pot0.wasUpdated()) {
			SynthAudio::codec.volume(_volume_pot0.read());
		}
	}
};
