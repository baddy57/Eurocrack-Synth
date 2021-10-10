#ifndef __MODULES_MIDIMONO_H__
#define __MODULES_MIDIMONO_H__ 1

#include "Module.h"
#include "Address.h"
#include <Audio.h>
#include <cstdint>
#include "CONTROLS_Potentiometer.h"
#include "CONTROLS_Switch.h"
#include "CONTROLS_Button.h"
#include "USBHost_t36.h"

#include <MIDI.h>
//#include <USBMIDI.h>
/*
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>*/

class MidiMono : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
	*/
		Switch source_sw0;
		Button chplus_btn0, chminus_btn1;
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