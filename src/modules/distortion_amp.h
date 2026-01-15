#pragma once

#include "../core/module.h"

namespace Distortion_amp_pins {
	enum inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN = 23 };
	enum outputs { OUT };
}

/// @brief implements distortion using amplification
class Distortion_amp : public Module {
private:
	Potentiometer _gain_pot0;
	Switch _bypass;
	AudioAmplifier _pre, _post;

	ModuleOutput out;
	ModuleInput in;

public:
	Distortion_amp() = delete;

	inline Distortion_amp(const Address& a)
		: Module(a)
		, _gain_pot0(a, Distortion_amp_pins::POT0, 4700)
		, _bypass(a, Distortion_amp_pins::BYPASS_SW)
		, out(a, Distortion_amp_pins::OUT, Distortion_amp_pins::OUT_D, _post, 0, "Distortion_amp OUT")
		, in(a, Distortion_amp_pins::IN, Distortion_amp_pins::IN_D, _pre, 0, "Distortion_amp IN")
	{
		internalConns.push_back(new AudioConnection(_pre, 0, _post, 0));

		_gain_pot0.setRange(0, 1, EXP);
	}

	inline void updateValues() override {
		if (_bypass.wasUpdated()) {
			if (!_bypass.b_read()) {
				_pre.gain(1.f);
				_post.gain(1.f);
				return;
			}
		}

		if (_gain_pot0.wasUpdated()) {
			float val = _gain_pot0.read();
			_pre.gain(val * 100);
			_post.gain(0.1 * val);
		}
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::DISTORTION_AMP; }
	
	const char* getModuleName() const override { return "DISTORTION_AMP"; }
	
#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back({"GAIN", Distortion_amp_pins::POT0, TestControlType::POTENTIOMETER, &_gain_pot0});
		digital.push_back({"BYPASS_SW", Distortion_amp_pins::BYPASS_SW, TestControlType::SWITCH, nullptr});
		digital.push_back({"IN_JK", Distortion_amp_pins::IN_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"OUT_JK", Distortion_amp_pins::OUT_D, TestControlType::JACK_DETECTOR, nullptr});
	}
#endif
};
