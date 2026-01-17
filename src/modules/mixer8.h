#pragma once

#include "../core/module.h"

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

	ModuleOutput outMono;
	ModuleOutput outStereoL;
	ModuleOutput outStereoR;
	ModuleInput ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7;

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
		, outMono(a, Mixer8_pins::OUT_MONO, Mixer8_pins::OUT_MONO_D, _mono_out, 0, "MIXR_OUT_MONO")
		, outStereoL(a, Mixer8_pins::OUT_STEREO_L, Mixer8_pins::OUT_STEREO_L_D, _channels0_3, 0, "MIXR_OUT_L")
		, outStereoR(a, Mixer8_pins::OUT_STEREO_R, Mixer8_pins::OUT_STEREO_R_D, _channels4_7, 0, "MIXR_OUT_R")
		, ch0(a, Mixer8_pins::CH0, Mixer8_pins::CH0_D, _channels0_3, 0, "CH0")
		, ch1(a, Mixer8_pins::CH1, Mixer8_pins::CH1_D, _channels0_3, 1, "CH1")
		, ch2(a, Mixer8_pins::CH2, Mixer8_pins::CH2_D, _channels0_3, 2, "CH2")
		, ch3(a, Mixer8_pins::CH3, Mixer8_pins::CH3_D, _channels0_3, 3, "CH3")
		, ch4(a, Mixer8_pins::CH4, Mixer8_pins::CH4_D, _channels4_7, 0, "CH4")
		, ch5(a, Mixer8_pins::CH5, Mixer8_pins::CH5_D, _channels4_7, 1, "CH5")
		, ch6(a, Mixer8_pins::CH6, Mixer8_pins::CH6_D, _channels4_7, 2, "CH6")
		, ch7(a, Mixer8_pins::CH7, Mixer8_pins::CH7_D, _channels4_7, 3, "CH7")
	{
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

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::MIXER8; }
	
	const char* getModuleName() const override { return "Mixer8"; }

	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back(TestControlInfo::createAnalog("GAIN0", _gain_pot0.getModulePin(), &_gain_pot0));
		analog.push_back(TestControlInfo::createAnalog("GAIN1", _gain_pot1.getModulePin(), &_gain_pot1));
		analog.push_back(TestControlInfo::createAnalog("GAIN2", _gain_pot2.getModulePin(), &_gain_pot2));
		analog.push_back(TestControlInfo::createAnalog("GAIN3", _gain_pot3.getModulePin(), &_gain_pot3));
		analog.push_back(TestControlInfo::createAnalog("GAIN4", _gain_pot4.getModulePin(), &_gain_pot4));
		analog.push_back(TestControlInfo::createAnalog("GAIN5", _gain_pot5.getModulePin(), &_gain_pot5));
		analog.push_back(TestControlInfo::createAnalog("GAIN6", _gain_pot6.getModulePin(), &_gain_pot6));
		analog.push_back(TestControlInfo::createAnalog("GAIN7", _gain_pot7.getModulePin(), &_gain_pot7));

		// Jack detectors are shown in socket section
	}

	void getTestSockets(std::vector<TestSocketInfo>& sockets) override {
		sockets.push_back(TestSocketInfo::createInput(ch0.base));
		sockets.push_back(TestSocketInfo::createInput(ch1.base));
		sockets.push_back(TestSocketInfo::createInput(ch2.base));
		sockets.push_back(TestSocketInfo::createInput(ch3.base));
		sockets.push_back(TestSocketInfo::createInput(ch4.base));
		sockets.push_back(TestSocketInfo::createInput(ch5.base));
		sockets.push_back(TestSocketInfo::createInput(ch6.base));
		sockets.push_back(TestSocketInfo::createInput(ch7.base));
		sockets.push_back(TestSocketInfo::createOutput(outMono.base));
		sockets.push_back(TestSocketInfo::createOutput(outStereoL.base));
		sockets.push_back(TestSocketInfo::createOutput(outStereoR.base));
	}
	#endif
};
