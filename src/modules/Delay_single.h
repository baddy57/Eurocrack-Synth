#pragma once
#include "../core/Module.h"

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

public:
	Delay_single() = delete;

	inline Delay_single(const Address& a)
		: Module(a)
		, pot0(a, Delay_single_pins::POT0, 4700)
	{
		using namespace Delay_single_pins;

		inputSockets.push_back(std::make_shared<InputSocket>(a, IN0, IN0_D, delay, 0, "delay"));
		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT0, OUT0_D, delay, 0, "delay"));

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
};
