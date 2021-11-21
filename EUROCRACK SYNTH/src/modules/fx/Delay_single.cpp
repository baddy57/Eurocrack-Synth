#include "Delay_single.h"

namespace {
	//in case of big module, use fix(pin) with inputs

	const uint8_t
	//inputs
		POT0(17),
		IN0(23),
		IN0_D(19),
		OUT0_D(20),
	//outputs
		OUT0(0);
}

Delay_single::Delay_single(const Address& a)
	:	Module(a)
	,	pot0	(a, POT0,4700)
{
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

void Delay_single::updateValues() {
	if (pot0.wasUpdated()) {
		float r = pot0.read();

		delay.delay(0, r);
	}
}