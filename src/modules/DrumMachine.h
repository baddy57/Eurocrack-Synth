#pragma once

#include "../core/Module.h"
#include "../sw_components/GateIn.h"
#include <MIDI.h>
#include "../samples/D_samples.h"

extern USBHost usbHost;
extern MIDIDevice midiOnUsbHost;

const uint_fast8_t banks = 1;

struct Voice {
	Button b;
	AudioSynthWavetable wavetable;
	uint_fast8_t bank;
	uint_fast8_t sample;
	uint_fast8_t first_sample;
	uint_fast8_t last_sample;
	float gain;
	const AudioSynthWavetable::instrument_data* active_sf2;

	Voice(Address a, uint_fast8_t i) : b(a, i) {
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

namespace DrumMachine_pins {
	enum inputs { _B0=8, _B1, _B2, _B3, _B4, _B5, _B6, _B7,
				  PAN, GAIN,
				  S0, S1, S2, S3,
				  CHMINUS, CHPLUS,
				  OUT_L_D=29, OUT_R_D, OUT_M_D };
	enum outputs { OUT_L, OUT_R, OUT_M };
}

extern std::vector<Voice*> DrumMachine_voices;

class DrumMachine : public Module {
private:
	midi::SerialMIDI<HardwareSerial> serialTransport;
	midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;
	Button s0, s1, s2, s3;
	Button chplus, chminus;

	Potentiometer pan_pot;
	Potentiometer gain_pot;
	Output left;
	Output right;
	Output mono;

	AudioMixer4* mxr[9];

	static uint_fast8_t _channel;

public:
	DrumMachine() = delete;

	inline DrumMachine(const Address& a)
		: Module(a)
		, serialTransport(Serial1)
		, MIDI(serialTransport)
		, s0(a, DrumMachine_pins::S0), s1(a, DrumMachine_pins::S0), s2(a, DrumMachine_pins::S2), s3(a, DrumMachine_pins::S3)
		, chplus(a, DrumMachine_pins::CHPLUS)
		, chminus(a, DrumMachine_pins::CHMINUS)
		, pan_pot(a, DrumMachine_pins::PAN)
		, gain_pot(a, DrumMachine_pins::GAIN)
		, left(a, DrumMachine_pins::OUT_L, DrumMachine_pins::OUT_L_D, *mxr[6], 0, "drum left")
		, right(a, DrumMachine_pins::OUT_R, DrumMachine_pins::OUT_R_D, *mxr[7], 0, "drum right")
		, mono(a, DrumMachine_pins::OUT_M, DrumMachine_pins::OUT_M_D, *mxr[8], 0, "drum mono")
	{
		using namespace DrumMachine_pins;

		for (uint_fast8_t i = 0; i < 9; ++i)
			mxr[i] = new AudioMixer4();

		DrumMachine_voices.push_back(new Voice(a, _B0));
		DrumMachine_voices.push_back(new Voice(a, _B1));
		DrumMachine_voices.push_back(new Voice(a, _B2));
		DrumMachine_voices.push_back(new Voice(a, _B3));
		DrumMachine_voices.push_back(new Voice(a, _B4));
		DrumMachine_voices.push_back(new Voice(a, _B5));
		DrumMachine_voices.push_back(new Voice(a, _B6));
		DrumMachine_voices.push_back(new Voice(a, _B7));

		for (uint_fast8_t i = 0; i < 4; ++i)
			for (uint_fast8_t j = 0; j < 3; ++j) {
				internalConns.push_back(new AudioConnection(DrumMachine_voices[i]->wavetable, 0, *(mxr[j % 3 * 2]), i));
				internalConns.push_back(new AudioConnection(DrumMachine_voices[i + 4]->wavetable, 0, *(mxr[j % 3 * 2 + 1]), i + 4));
			}

		for (uint_fast8_t i = 0; i < 6; ++i)
			internalConns.push_back(new AudioConnection(*(mxr[i]), 0, *(mxr[i / 2 + 6]), i % 2));

		_channel = 2;

		MIDI.setHandleNoteOn(handleNoteOn);
		MIDI.setHandleNoteOff(handleNoteOff);
		usbMIDI.setHandleNoteOn(handleNoteOn);
		usbMIDI.setHandleNoteOff(handleNoteOff);
		midiOnUsbHost.setHandleNoteOn(handleNoteOn);
		midiOnUsbHost.setHandleNoteOff(handleNoteOff);

		MIDI.begin(_channel);

		pan_pot.setRange(0, 1, LIN);
		gain_pot.setRange(0, 1, EXP);
	}

	inline void updateValues() override {
		using namespace DrumMachine_pins;

		if (_channel < 16 && chplus.wasUpdated()) {
			if (chplus.isPressed()) {
				_channel++;
				tft.print("ch ");
				tft.println(_channel);
			}
		}
		if (_channel > 1 && chminus.wasUpdated()) {
			if (chminus.isPressed()) {
				_channel--;
				tft.print("ch ");
				tft.println(_channel);
			}
		}

		for (uint_fast8_t i = 0; i < 8; ++i) {
			uint_fast8_t x = 1;
			if (i < 4) x = 0;
			if (DrumMachine_voices[i]->b.isPressed()) {
				if (pan_pot.wasUpdated()) {
					float g = DrumMachine_voices[i]->gain;
					float r = pan_pot.read();
					mxr[x]->gain(i % 4, g * (1.0 - r));
					mxr[x + 2]->gain(i % 4, g * r);
				}
				if (gain_pot.wasUpdated()) {
					DrumMachine_voices[i]->setGain(gain_pot.read());
					mxr[x + 4]->gain(i % 4, DrumMachine_voices[i]->gain);
				}
				if (DrumMachine_voices[i]->bank > 0 && s0.wasUpdated() && s0.isPressed()) {
					DrumMachine_voices[i]->prevBank();
				}
				if (DrumMachine_voices[i]->bank < banks - 1 && s1.wasUpdated() && s1.isPressed()) {
					DrumMachine_voices[i]->nextBank();
				}
				if (DrumMachine_voices[i]->sample > 0 && s2.wasUpdated() && s2.isPressed()) {
					DrumMachine_voices[i]->prevSample();
					handleNoteOn(_channel, i + 44, 127);
				}
				if (DrumMachine_voices[i]->sample < DrumMachine_voices[i]->last_sample && s3.wasUpdated() && s3.isPressed()) {
					DrumMachine_voices[i]->nextSample();
					handleNoteOn(_channel, i + 44, 127);
				}
			}
		}
		usbHost.Task();
		midiOnUsbHost.read();
	}

	inline static void handleNoteOn(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity) {
		if (inChannel == _channel && inVelocity != 0) {
			if (inNote >= 44 && inNote <= 51) {
				DrumMachine_voices[inNote - 44]->wavetable.playNote(DrumMachine_voices[inNote - 44]->sample, inVelocity);
			}
		}
	}

	inline static void handleNoteOff(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity) {
		if (inChannel == _channel)
			if (inNote >= 44 && inNote <= 51)
				DrumMachine_voices[inNote - 44]->wavetable.stop();
	}
};
