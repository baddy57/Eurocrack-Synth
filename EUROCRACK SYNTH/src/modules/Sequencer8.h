#ifndef _SEQUENCER_MODULE_H
#define _SEQUENCER_MODULE_H

#include "../Module.h"

struct SequencerStep {
	uint note = 0;
	uint octave = 0;
	uint length = 0;
	uint velocity = 0;
	int delay = 0;
	float frequency = 0;
	float probability = 0;
	bool modified = false;
	uint attackTime_ms;
	uint releaseTime_ms;
};

class Sequencer8 : public Module {
public:
	Sequencer8(const Address&);
	void updateValues();

	AudioSynthWaveformDc gateStream;
	AudioSynthWaveformDc cvStream;
	AudioSynthWaveformDc velocityStream;
private:
	Fader *faders[8];
	Button *buttons[8];
	RotarySwitch fnSelector;
	Potentiometer manualClock;

	Input clockIn;
	Output gateOut;
	Output cvOut;
	Output velocityOut;

	//Led clockLed;

	SequencerStep steps[8];

	GateIn clockInReader;

	uint timer;
	uint lastBeat;
	uint tempo_ms;
	uint currentStep;
	uint bpm;
	uint (*scale)(uint,uint);
	uint root;
	uint sequenceLength_ms;

	int chosenStep = -1;
	int stepParam = -1;
	//uint activeFn;

	void getClockFromInput();
	void setClock();
	void setNote(uint track, float note);
	void playStep();

};

#endif