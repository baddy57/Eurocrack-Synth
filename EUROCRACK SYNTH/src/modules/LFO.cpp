#include "LFO.h"


namespace {
	const int
		SEL = 24,
		POT0 = 25,
		POT1 = 26,
		CV_D = 27,
		CV_IN = 28,
		SYNC_D = 29,
		SYNC_IN = 30,
		OUT_D = 31,
		OUT = 0;
	const int COMMON_PULLUP = 4700;
	enum { SINE, TRIANGLE, SQUARE, SAW, RAMP };
	const int PJRC_WAVES[5]{ WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH_REVERSE };
	std::string wave_names[5]{ "SINE", "TRIANGLE", "SQUARE", "SAW", "RAMP" };
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
	freqPot.setRange(0, 20, EXP);
	cvPot.setRange(-5, 5, LIN);
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
		freq = freqPot.read();
		wave.frequency(freq);
	}

	//cv
	if (cvPot.wasUpdated()) {
		float cv = cvPot.read();
		wave.frequencyModulation(cv);
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