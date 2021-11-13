#ifndef __MODULES_MIDIMONO_H__
#define __MODULES_MIDIMONO_H__

#include "../Module.h"
#include "USBHost_t36.h"
#include <MIDI.h>

//#include <USBMIDI.h>

class MidiMono : public Module {
	private:
		Switch 
			source_sw0;

		Button 
			chplus_btn0, 
			chminus_btn1;

		midi :: MidiInterface<HardwareSerial> MIDI;
	public:
		MidiMono (const Address&);
		void updateValues();

		static AudioSynthWaveformDc _gate;
		static AudioSynthWaveformDc _cv;
		static AudioSynthWaveformDc _vel;

		static uint_fast8_t _channel;
		
		static void handleNoteOn(uint8_t, uint8_t, uint8_t);
		static void handleNoteOff(uint8_t, uint8_t, uint8_t);
};

#endif