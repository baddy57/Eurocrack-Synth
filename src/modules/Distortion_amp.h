#pragma once

#include "../core/Module.h"

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

public:
	Distortion_amp() = delete;

	inline Distortion_amp(const Address& a)
		: Module(a)
		, _gain_pot0(a, Distortion_amp_pins::POT0, 4700)
		, _bypass(a, Distortion_amp_pins::BYPASS_SW)
	{
		using namespace Distortion_amp_pins;

		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT, OUT_D, _post, 0, "Distortion_amp OUT"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, IN, IN_D, _pre, 0, "Distortion_amp IN"));

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
};
