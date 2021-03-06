#ifndef __MODULES_MIDI_H__
#define __MODULES_MIDI_POLY_H__ 1

#include "../Module.h"
#include "../Address.h"
#include <Audio.h>
#include <cstdint>
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include <MIDI.h>
/*
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>*/

class MidiPoly : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
	*/
		static void handleNoteOn(uint8_t, uint8_t, uint8_t);
		static void handleNoteOff(uint8_t, uint8_t, uint8_t);
		static AudioSynthWaveformDc* _gate1;
		static AudioSynthWaveformDc* _note1;
		midi :: MidiInterface<HardwareSerial> MIDI;
	public:
		MidiPoly (const Address&);
		void updateValues();
};

#endif