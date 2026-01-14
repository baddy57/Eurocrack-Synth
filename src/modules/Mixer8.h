#pragma once

#include "../core/Module.h"

namespace Mixer8_pins {
	enum inputs { CH0_D=16, CH1_D, CH2_D, CH3_D, CH4_D, CH5_D, CH6_D, CH7_D,
				  CH7=24, CH6, CH5, CH4, CH3, CH2, CH1, CH0,
				  OUT_STEREO_L_D=32, OUT_STEREO_R_D, SW, OUT_MONO_D };
	const uint_fast8_t GAIN0 = fix(39), GAIN1 = fix(38), GAIN2 = fix(37), GAIN3 = fix(36);
	const uint_fast8_t GAIN4 = fix(35), GAIN5 = fix(34), GAIN6 = fix(33), GAIN7 = fix(32);
	enum outputs { OUT_MONO, OUT_STEREO_L, OUT_STEREO_R };
}

class Mixer8 : public Module {
private:
	Potentiometer _gain_pot0, _gain_pot1, _gain_pot2, _gain_pot3;
	Potentiometer _gain_pot4, _gain_pot5, _gain_pot6, _gain_pot7;

	AudioMixer4 _channels0_3, _channels4_7, _mono_out;

public:
	Mixer8() = delete;

	inline Mixer8(const Address& a)
		: Module(a)
		, _gain_pot0(a, Mixer8_pins::GAIN0, 4700.f)
		, _gain_pot1(a, Mixer8_pins::GAIN1, 4700.f)
		, _gain_pot2(a, Mixer8_pins::GAIN2, 4700.f)
		, _gain_pot3(a, Mixer8_pins::GAIN3, 4700.f)
		, _gain_pot4(a, Mixer8_pins::GAIN4, 4700.f)
		, _gain_pot5(a, Mixer8_pins::GAIN5, 4700.f)
		, _gain_pot6(a, Mixer8_pins::GAIN6, 4700.f)
		, _gain_pot7(a, Mixer8_pins::GAIN7, 4700.f)
	{
		using namespace Mixer8_pins;

		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_MONO, OUT_MONO_D, _mono_out, 0, "MIXR_OUT_MONO"));
		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_STEREO_L, OUT_STEREO_L_D, _channels0_3, 0, "MIXR_OUT_L"));
		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_STEREO_R, OUT_STEREO_R_D, _channels4_7, 0, "MIXR_OUT_R"));

		inputSockets.push_back(std::make_shared<InputSocket>(a, CH0, CH0_D, _channels0_3, 0, "CH0"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH1, CH1_D, _channels0_3, 1, "CH1"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH2, CH2_D, _channels0_3, 2, "CH2"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH3, CH3_D, _channels0_3, 3, "CH3"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH4, CH4_D, _channels4_7, 0, "CH4"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH5, CH5_D, _channels4_7, 1, "CH5"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH6, CH6_D, _channels4_7, 2, "CH6"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CH7, CH7_D, _channels4_7, 3, "CH7"));

		internalConns.push_back(new AudioConnection(_channels0_3, 0, _mono_out, 0));
		internalConns.push_back(new AudioConnection(_channels4_7, 0, _mono_out, 1));

		for (int i = 0; i < 4; i++) {
			_channels0_3.gain(i, 0);
			_channels4_7.gain(i, 0);
		}

		_gain_pot0.setRange(0, 1, EXP);
		_gain_pot1.setRange(0, 1, EXP);
		_gain_pot2.setRange(0, 1, EXP);
		_gain_pot3.setRange(0, 1, EXP);
		_gain_pot4.setRange(0, 1, EXP);
		_gain_pot5.setRange(0, 1, EXP);
		_gain_pot6.setRange(0, 1, EXP);
		_gain_pot7.setRange(0, 1, EXP);
	}

	inline void updateValues() override {
		if (_gain_pot0.wasUpdated())
			_channels0_3.gain(0, _gain_pot0.read());
		if (_gain_pot1.wasUpdated())
			_channels0_3.gain(1, _gain_pot1.read());
		if (_gain_pot2.wasUpdated())
			_channels0_3.gain(2, _gain_pot2.read());
		if (_gain_pot3.wasUpdated())
			_channels0_3.gain(3, _gain_pot3.read());
		if (_gain_pot4.wasUpdated())
			_channels4_7.gain(0, _gain_pot4.read());
		if (_gain_pot5.wasUpdated())
			_channels4_7.gain(1, _gain_pot5.read());
		if (_gain_pot6.wasUpdated())
			_channels4_7.gain(2, _gain_pot6.read());
		if (_gain_pot7.wasUpdated())
			_channels4_7.gain(3, _gain_pot7.read());
	}
};
