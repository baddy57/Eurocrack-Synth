#include "LFO.h"


namespace {
	const int
		SEL = 0,
		POT0 = 0,
		POT1 = 0,
		CV_IN = 0,
		CV_D = 0,
		SYNC_IN = 0,
		SYNC_D = 0,
		OUT = 0,
		OUT_D = 0;
	const int COMMON_PULLUP = 4700;
	enum { SINE, TRIANGLE, SQUARE, SAW, RAMP };
	const int PJRC_WAVES[5]{ WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH_REVERSE };
};

//ctor
LFO :: LFO (const Address& a)
	:	Module(a)
	,	waveShapeSel (a, SEL, 5, COMMON_PULLUP)
	,   freqPot (a, POT0, COMMON_PULLUP)
	,	cvPot (a, POT1, COMMON_PULLUP)
	,	syncIn(a, SYNC_IN, SYNC_D, sync, 0, "LFO SYNC")
	,	cvIn(a, CV_IN, CV_D, wave, 0, "LFO FM")
	,	out(a, OUT, OUT_D, wave, 0, "LFO OUT")
{
	//inputSockets.push_back(std::make_shared<InputSocket>(a, CV_IN, CV_D, wave, 0, "LFO FM"));
	//inputSockets.push_back(std::make_shared<InputSocket>(a, SYNC_IN, SYNC_D, sync, 0, "LFO SYNC"));
	//outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT,OUT_D, wave, 0, "LFO OUT"));
}


void LFO :: updateValues() {
	//waveshape selector
	if (waveShapeSel.wasUpdated()) {
		int i = waveShapeSel.read();
		wave.begin(1.f, freq, PJRC_WAVES[i]);
	}

	//frequency
	if (freqPot.wasUpdated()) {
		freq = freqPot.read(0, 50);
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