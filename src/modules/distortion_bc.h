#pragma once

#include "../core/module.h"

namespace Distortion_bc_pins {
	enum inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN = 23 };
	enum outputs { OUT };
}

/// @brief implements distortion using bitcrusher
class Distortion_bc : public Module {
private:
	Potentiometer
		_bits_pot0,
		_sampleRate_pot1;
	Switch _bypass;
	AudioEffectBitcrusher _dist;

	ModuleOutput out;
	ModuleInput in;

public:
	Distortion_bc() = delete;

	inline Distortion_bc(const Address& a)
		: Module(a)
		, _bits_pot0(a, Distortion_bc_pins::POT0, 4700)
		, _sampleRate_pot1(a, Distortion_bc_pins::POT1, 4700)
		, _bypass(a, Distortion_bc_pins::BYPASS_SW)
		, out(a, Distortion_bc_pins::OUT, Distortion_bc_pins::OUT_D, _dist, 0, "Distortion_bc OUT")
		, in(a, Distortion_bc_pins::IN, Distortion_bc_pins::IN_D, _dist, 0, "Distortion_bc IN")
	{
		_bits_pot0.setRange(16, 1, LIN);
		_sampleRate_pot1.setRange(44100, 1, EXP);
	}

	inline void updateValues() override {
		if (_bypass.wasUpdated()) {
			if (!_bypass.b_read()) {
				_dist.bits(16);
				_dist.sampleRate(44100);
				return;
			}
		}

		if (_bits_pot0.wasUpdated()) {
			uint8_t val = _bits_pot0.read();
			_dist.bits(val);
		}
		if (_sampleRate_pot1.wasUpdated()) {
			float val = _sampleRate_pot1.read();
			_dist.sampleRate(val);
		}
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::DISTORTION; }
	
	const char* getModuleName() const override { return "DISTORTION_BC"; }
	
#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back({"BITS", Distortion_bc_pins::POT0, TestControlType::POTENTIOMETER, &_bits_pot0});
		analog.push_back({"SAMPLE_RATE", Distortion_bc_pins::POT1, TestControlType::POTENTIOMETER, &_sampleRate_pot1});
		digital.push_back({"BYPASS_SW", Distortion_bc_pins::BYPASS_SW, TestControlType::SWITCH, nullptr});
		digital.push_back({"IN_JK", Distortion_bc_pins::IN_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"OUT_JK", Distortion_bc_pins::OUT_D, TestControlType::JACK_DETECTOR, nullptr});
	}
#endif
};
