#include "Delay_multi.h"

namespace {
	//in case of big module, use fix(pin) with inputs

	const uint8_t
	//inputs
		POT0(17),
		POT1(18),
		IN0(23),
		IN0_D(19),
		OUT0_D(20),
	//outputs
		OUT0(0);
}

Delay_multi::Delay_multi(const Address& a)
	:	Module	(a)
	,	pot0	(a, POT0, 4700)
	,	pot1	(a, POT1,4700)
{
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN0, IN0_D, delay, 0, "delay"));
	internalConns.push_back(new AudioConnection(delay, 0, intmixer0, 0));
	internalConns.push_back(new AudioConnection(delay, 1, intmixer0, 1));
	internalConns.push_back(new AudioConnection(delay, 2, intmixer0, 2));
	internalConns.push_back(new AudioConnection(delay, 3, intmixer0, 3));	
	internalConns.push_back(new AudioConnection(delay, 4, intmixer1, 0));
	internalConns.push_back(new AudioConnection(delay, 5, intmixer1, 1));
	internalConns.push_back(new AudioConnection(delay, 6, intmixer1, 2));
	internalConns.push_back(new AudioConnection(delay, 7, intmixer1, 3));
	internalConns.push_back(new AudioConnection(intmixer0, 0, outmix, 0));
	internalConns.push_back(new AudioConnection(intmixer1, 0, outmix, 1));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT0, OUT0_D, outmix, 0, "delay"));
	intmixer0.gain(0, 0.64);
	intmixer0.gain(1, 0.32);
	intmixer0.gain(2, 0.16);
	intmixer0.gain(3, 0.08); 
	intmixer1.gain(0, 0.04);
	intmixer1.gain(1, 0.02);
	intmixer1.gain(2, 0.01);
	intmixer1.gain(3, 0.005);
	outmix.gain(0, 0.6);
	outmix.gain(1, 0.6);
	pot0.setRange(0, 2000, EXP);
}

void Delay_multi::updateValues() {
	if (pot0.wasUpdated()) {
		float r = pot0.read();
		delay.delay(0, r);
		//delay.delay(1, r*2);
		//delay.delay(2, r*3);
		//delay.delay(3, r*4);
		//delay.delay(4, r*5);
		//delay.delay(5, r*6);
		//delay.delay(6, r*7);
		//delay.delay(7, r*8);
	}
	//pot1 mutes taps >n
	//pot2 sets ratio
}