#pragma once
#include "../Module.h"

struct SequencerStep {
	Fader* fader;
	uint note;
	uint octave;
};

class Sequencer8 : public Module {
private:
	Button chplus_btn0;
	Button chminus_btn1;

	SequencerStep steps[8];

	Potentiometer manualClock;

	uint timer;
	uint lastBeat;
	uint tempo_ms;
	uint currentStep;

	uint* scale;
	uint root;

	Input clockIn;
	GateIn clockInReader;
	Output gateOut;
	Output cvOut;
	//Output velocityOut;

	void getClockFromInput();
	void setClock();
	void setNote(uint track, float note);
	void playStep();

public:
	Sequencer8(const Address&);
	void updateValues();

	AudioSynthWaveformDc gateStream;
	AudioSynthWaveformDc cvStream;
	AudioSynthWaveformDc velocityStream;
};

