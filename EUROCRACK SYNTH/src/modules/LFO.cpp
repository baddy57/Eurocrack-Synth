#include "LFO.h"


namespace {
	const int
		SEL = 0,
		SW = 0,
		POT0 = 0,
		POT1 = 0,
		CV_IN = 0,
		CV_D = 0,
		OUT = 0,
		OUT_D = 0;
	const int PULLUP_RES_VAL = 4700;
	enum { SINE, TRIANGLE, SQUARE, SAW, RAMP };
	const int PJRC_WAVES[5]{ WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH_REVERSE };
	const int LOW_RANGE = 20, HIGH_RANGE = 50;
};

//ctor
LFO :: LFO (const Address& a)
	:	Module(a)
	,	waveShapeSel	(a, SEL, 5, PULLUP_RES_VAL)
	,   freqPot(a, POT0, PULLUP_RES_VAL)
	,	cvPot (a, POT1, PULLUP_RES_VAL)
	,	rangeSw (a, SW)
{
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV_IN, CV_D, wave, 0, "LFO FM"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT,OUT_D, wave, 0, "LFO OUT"));
}


void LFO :: updateValues() {

	//range selection switch
	if(rangeSw.wasUpdated()){
		if (rangeSw.b_read())
			maxRange = LOW_RANGE;
		else
			maxRange = HIGH_RANGE;
	}

	//waveshape selector
	if (waveShapeSel.wasUpdated()) {
		int i = waveShapeSel.read();
		wave.begin(1.f, freq, PJRC_WAVES[i]);
	}

	//frequency
	if (freqPot.wasUpdated()) {
		freq = freqPot.read(0, maxRange);
		wave.frequency(freq);
	}

	//cv
	if (cvPot.wasUpdated()) {
		wave.frequencyModulation(cvPot.read(-5, 5));
	}

}

/*sync: 
private: GateIn link
inputSockets.pus_back(link...)
if link.read()
	wave.begin(1.0, freq, getcorresponding(currentWave))
///////// NO /////internalConns.push_back(new AudioConnection


	switch (waveShapeSel.read()) {
	case SINE:
		if (currentWave == SINE) break;
		wave.begin(1.f, freq, WAVEFORM_SINE);
		currentWave = SINE;
		break;
	case TRIANGLE:
		if (currentWave == TRIANGLE) break;
		wave.begin(1.f, freq, WAVEFORM_TRIANGLE);
		currentWave = TRIANGLE;
		break;
	case SQUARE:
		if (currentWave == SQUARE) break;
		wave.begin(1.f, freq, WAVEFORM_SQUARE);
		currentWave = SQUARE;
		break;
	case SAW:
		if (currentWave == SAW) break;
		wave.begin(1.f, freq, WAVEFORM_SAWTOOTH);
		currentWave = SAW;
		break;
	case RAMP:
		if (currentWave == RAMP) break;
		wave.begin(1.f, freq, WAVEFORM_SAWTOOTH_REVERSE);
		currentWave = RAMP;
		break;
	}


*/