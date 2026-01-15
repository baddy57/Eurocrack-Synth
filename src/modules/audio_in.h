#pragma once

#include "../core/module.h"
#include "../services/synth_audio.h"

namespace AudioIn_pins {
	enum inputs { MIC_GAIN=16, MIC_IN_D, SW, LINE_IN_L_D, LINE_IN_R_D, USB_IN_L_D, USB_IN_R_D=23 };
	enum outputs { LINE_IN_L=2, MIC_IN, LINE_IN_R, USB_IN_L, USB_IN_R=7 };
}

class AudioIn : public Module {
private:
	AudioInputI2S _lineIn;
	AudioAmplifier _preamp;
	AudioInputUSB _usbIn;
	Potentiometer _micGain;
	ModuleOutput lineInL;
	ModuleOutput lineInR;
	ModuleOutput usbInL;
	ModuleOutput usbInR;

public:
	inline AudioIn(const Address& a)
		: Module(a)
		, _micGain(a, 16, 4700)
		, lineInL(a, AudioIn_pins::LINE_IN_L, AudioIn_pins::LINE_IN_L_D, _preamp, 0, "LINE_IN_L")
		, lineInR(a, AudioIn_pins::LINE_IN_R, AudioIn_pins::LINE_IN_R_D, _lineIn, 1, "LINE_IN_R")
		, usbInL(a, AudioIn_pins::USB_IN_L, AudioIn_pins::USB_IN_L_D, _usbIn, 0, "USB_IN_L")
		, usbInR(a, AudioIn_pins::USB_IN_R, AudioIn_pins::USB_IN_R_D, _usbIn, 1, "USB_IN_R")
	{
		SynthAudio::codec.adcHighPassFilterDisable();
		SynthAudio::codec.lineInLevel(1);
		internalConns.push_back(new AudioConnection(_lineIn, 0, _preamp, 0));
		_preamp.gain(10);
		_micGain.setRange(0, 1, EXP);
	}

	inline void updateValues() override {
	}

	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back({"MIC_GAIN", AudioIn_pins::MIC_GAIN, TestControlType::POTENTIOMETER, &_micGain});
		digital.push_back({"MIC_IN_JK", AudioIn_pins::MIC_IN_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"LINE_IN_L_JK", AudioIn_pins::LINE_IN_L_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"LINE_IN_R_JK", AudioIn_pins::LINE_IN_R_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"USB_IN_L_JK", AudioIn_pins::USB_IN_L_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"USB_IN_R_JK", AudioIn_pins::USB_IN_R_D, TestControlType::JACK_DETECTOR, nullptr});
	}
	#endif
};
