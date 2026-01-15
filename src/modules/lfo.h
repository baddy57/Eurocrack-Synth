#pragma once

#include "../core/module.h"
#include "../features/gate_in.h"
#include <string>

namespace LFO_pins {
	const int SEL = 24, POT0 = 25, POT1 = 26, CV_D = 27, CV_IN = 28;
	const int SYNC_D = 29, SYNC_IN = 30, OUT_D = 31, OUT = 0;
	const int COMMON_PULLUP = 4700;
	enum waves { SINE, TRIANGLE, SQUARE, SAW, RAMP };
	const int PJRC_WAVES[5] = { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH_REVERSE };
}

class LFO : public Module {
private:
	SelectorMulti waveShapeSel;
	Potentiometer freqPot;
	Potentiometer cvPot;

	ModuleInput syncIn;
	ModuleInput cvIn;
	ModuleOutput out;

	GateIn sync;
	int currentWave = -1;
	float freq = 10;

	AudioSynthWaveformModulated wave;

public:
	LFO() = delete;

	inline LFO(const Address& a)
		: Module(a)
		, waveShapeSel(a, LFO_pins::SEL, 5, LFO_pins::COMMON_PULLUP)
		, freqPot(a, LFO_pins::POT0, LFO_pins::COMMON_PULLUP)
		, cvPot(a, LFO_pins::POT1, LFO_pins::COMMON_PULLUP)
		, syncIn(a, LFO_pins::SYNC_IN, LFO_pins::SYNC_D, sync, 0, "LFO SYNC")
		, cvIn(a, LFO_pins::CV_IN, LFO_pins::CV_D, wave, 0, "LFO FM")
		, out(a, LFO_pins::OUT, LFO_pins::OUT_D, wave, 0, "LFO OUT")
	{
		freqPot.setRange(0, 20, EXP);
		cvPot.setRange(-5, 5, LIN);
	}

	inline void updateValues() override {
		using namespace LFO_pins;

		if (waveShapeSel.wasUpdated()) {
			int i = waveShapeSel.read();
			wave.begin(1.f, freq, PJRC_WAVES[i]);
		}

		if (freqPot.wasUpdated()) {
			freq = freqPot.read();
			wave.frequency(freq);
		}

		if (cvPot.wasUpdated()) {
			float cv = cvPot.read();
			wave.frequencyModulation(cv);
		}
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::LFO; }
	
	const char* getModuleName() const override { return "LFO"; }

	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		// analog.push_back(TestControlInfo::createAnalog("WAVE", waveShapeSel.getPinId(), TestControlType::SELECTOR_MULTI, &waveShapeSel)); //todo
		
		analog.push_back(TestControlInfo::createAnalog("FREQ", freqPot.getPinId(), &freqPot));
		analog.push_back(TestControlInfo::createAnalog("CV_AMT", cvPot.getPinId(), &cvPot));

		digital.push_back(TestControlInfo::createDigital("CV_JK", LFO_pins::CV_D, TestControlType::JACK_DETECTOR));
		digital.push_back(TestControlInfo::createDigital("SYNC_JK", LFO_pins::SYNC_D, TestControlType::JACK_DETECTOR));
		digital.push_back(TestControlInfo::createDigital("OUT_JK", LFO_pins::OUT_D, TestControlType::JACK_DETECTOR));
	}
	#endif
};
