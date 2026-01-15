#pragma once

#include "../core/module.h"
#include <vector>
#include <Audio.h>
#include "../samples/d_samples.h"

struct DrumMachineVoice {
	Button b;
	AudioSynthWavetable wavetable;
	uint_fast8_t bank;
	uint_fast8_t sample;
	uint_fast8_t first_sample;
	uint_fast8_t last_sample;
	float gain;
	const AudioSynthWavetable::instrument_data* active_sf2;

	DrumMachineVoice(Address a, uint_fast8_t i) : b(a, i) {
		gain = 1.0;
		setBank(0);
	}

	void setBank(uint_fast8_t newBank) {
		bank = newBank;
		wavetable.setInstrument(D);
		active_sf2 = &D;
		first_sample = active_sf2->sample_note_ranges[0];
		last_sample = first_sample + active_sf2->sample_count;
		sample = first_sample;
	}

	void nextBank() { setBank(bank + 1); }
	void prevBank() { setBank(bank - 1); }
	void setSample(uint_fast8_t newSample) { sample = newSample; }
	void nextSample() { setSample(sample + 1); }
	void prevSample() { setSample(sample - 1); }
	void setGain(float g) { gain = g; }
};

extern std::vector<DrumMachineVoice*> DrumMachine_voices;
