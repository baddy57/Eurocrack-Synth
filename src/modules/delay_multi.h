#pragma once
#include "../core/module.h"

namespace Delay_multi_pins {
	const uint8_t
		POT0 = 17,
		POT1 = 18,
		IN0 = 23,
		IN0_D = 19,
		OUT0_D = 20,
		OUT0 = 0;
}

/// @brief Multi-tap delay module
class Delay_multi : public Module {
protected:
	Potentiometer pot0, pot1;
	AudioEffectDelay delay;
	AudioMixer4 intmixer0, intmixer1, outmix;

	ModuleInput in;
	ModuleOutput out;

public:
	Delay_multi() = delete;

	inline Delay_multi(const Address& a)
		: Module(a)
		, pot0(a, Delay_multi_pins::POT0, 4700)
		, pot1(a, Delay_multi_pins::POT1, 4700)
		, in(a, Delay_multi_pins::IN0, Delay_multi_pins::IN0_D, delay, 0, "delay")
		, out(a, Delay_multi_pins::OUT0, Delay_multi_pins::OUT0_D, outmix, 0, "delay")
	{
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

	inline void updateValues() override {
		if (pot0.wasUpdated()) {
			float r = pot0.read();
			delay.delay(0, r);
			// Commented out in original:
			// delay.delay(1, r*2);
			// delay.delay(2, r*3);
			// ... etc
		}
		// TODO: pot1 mutes taps >n
		// TODO: pot2 sets ratio
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::DELAY; }
	
	const char* getModuleName() const override { return "DelayMulti"; }
	
	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		analog.push_back({"TIME", pot0.getPinId(), TestControlType::POTENTIOMETER, &pot0});
	}
	#endif
};
