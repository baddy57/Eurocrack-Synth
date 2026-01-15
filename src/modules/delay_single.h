#pragma once
#include "../core/module.h"

namespace Delay_single_pins {
	const uint8_t
		POT0 = 17,
		IN0 = 23,
		IN0_D = 19,
		OUT0_D = 20,
		OUT0 = 0;
}

/// @brief Single delay module
class Delay_single : public Module {
protected:
	Potentiometer pot0;
	AudioEffectDelay delay;

	ModuleInput in;
	ModuleOutput out;

public:
	Delay_single() = delete;

	inline Delay_single(const Address& a)
		: Module(a)
		, pot0(a, Delay_single_pins::POT0, 4700)
		, in(a, Delay_single_pins::IN0, Delay_single_pins::IN0_D, delay, 0, "delay")
		, out(a, Delay_single_pins::OUT0, Delay_single_pins::OUT0_D, delay, 0, "delay")
	{
		delay.disable(1);
		delay.disable(2);
		delay.disable(3);
		delay.disable(4);
		delay.disable(5);
		delay.disable(6);
		delay.disable(7);

		pot0.setRange(0, 2000, EXP);
	}

	inline void updateValues() override {
		if (pot0.wasUpdated()) {
			float r = pot0.read();
			delay.delay(0, r);
		}
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::DELAY_SINGLE; }
	
	const char* getModuleName() const override { return "DELAY_SINGLE"; }
	
#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back({"POT0", Delay_single_pins::POT0, TestControlType::POTENTIOMETER, &pot0});
		digital.push_back({"IN0_JK", Delay_single_pins::IN0_D, TestControlType::JACK_DETECTOR, nullptr});
		digital.push_back({"OUT0_JK", Delay_single_pins::OUT0_D, TestControlType::JACK_DETECTOR, nullptr});
	}
#endif
};
