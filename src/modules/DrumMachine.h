#ifndef __MODULES_DrumMachine_H__
#define __MODULES_DrumMachine_H__ 1

#include "Module.h"
#include "Address.h"
#include <Audio.h>
#include <cstdint>
#include "CONTROLS_Potentiometer.h"
#include "CONTROLS_Switch.h"
#include "CONTROLS_Button.h"
#include "GateIn.h"

#include <MIDI.h>
#include "D_samples.h"

const uint_fast8_t 	banks = 1;
//const AudioSynthWavetable :: instrument_data* _sf2[banks] = {&D};//////

struct Voice{
	Button b;
	AudioSynthWavetable wavetable;
	//AudioEffectEnvelope env;
	uint_fast8_t bank; //instrument_data object
	uint_fast8_t sample; //note
	uint_fast8_t first_sample; //note
	uint_fast8_t last_sample; //note
	float gain;
	const AudioSynthWavetable :: instrument_data* active_sf2;
	Voice(Address a, uint_fast8_t i): b(a,i){
		gain=1.0;
		setBank(0);
	};
	void setBank(uint_fast8_t newBank){
		bank=newBank;
		wavetable.setInstrument(D/**(_sf2[bank])*/);
		active_sf2 	= 	&D/*_sf2[bank]*/;
		first_sample =	active_sf2->sample_note_ranges[0];
		last_sample =	first_sample+active_sf2->sample_count;//first samples + number of samples //assuming samples are stored in a contiguous area of the keyboard
		sample		=	first_sample;
	}
	void nextBank(){setBank(bank+1);};
	void prevBank(){setBank(bank-1);};
	void setSample(uint_fast8_t newSample){	sample=newSample;	};
	void nextSample(){setSample(sample+1);};
	void prevSample(){setSample(sample-1);};
	void setGain(float g){	gain = g;}
};

class DrumMachine : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection*> internalConns;//if any;
	*/
		
		midi :: MidiInterface<HardwareSerial> MIDI;
		Button s0, s1, s2, s3;
		Button chplus, chminus;
		
		Potentiometer 	pan_pot,
						gain_pot;
		//Switch 	mono_stereo_sw0;//, _sw1;
		AudioMixer4* mxr[9];
		
		//<streamtype> im0, im1;
		
		static uint_fast8_t _channel;
		static void handleNoteOn(uint8_t, uint8_t, uint8_t);
		static void handleNoteOff(uint8_t, uint8_t, uint8_t);
		
	public:
	//	static std::vector<Voice*> voices;
		DrumMachine () = delete;
		DrumMachine (const Address&);
		void updateValues(); //(uint_fast8_t priority) ?
		
};
#endif