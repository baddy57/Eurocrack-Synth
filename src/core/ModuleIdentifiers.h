#pragma once

#include <Wire.h>

class ModuleIdentifiers
{
public:
	const static byte zero = 0b00000000;		// 0 = 0000 0000
	const static byte AUDIO_OUT = 0b00000001;	// 1 = 0000 0001
	const static byte AUDIO_IN = 0b00000010;	// 2 = 0000 0010
	const static byte MIDI_MODULE = 0b00000011; // 3 = 0000 0011
	const static byte VCO_ID = 0b00000100;		// 4 = 0000 0100
	const static byte LFO = 0b00000101;			// 5 = 0000 0101
	const static byte FILTER = 0b00000110;
	const static byte ADSR = 0b00000111;
	const static byte VCA = 0b00001000;
	const static byte MIXER8 = 0b00001001; // 9 = 00001001
	const static byte DISTORTION = 0b00001010;
	const static byte REVERB = 0b00001011; // 11 = 00001011
	const static byte DELAY = 0b00001100;
	const static byte LOOPER = 0b00001101;
	const static byte NOISE = 0b00001110;
	const static byte FLANGER = 0b00001111;
	const static byte DISTORTION_AMP = 0b00001111;
	const static byte DRUMS = 0b00010001; // 17 = 00010001
	const static byte INSTRUMENTS = 0b00010010;
	const static byte DELAY_SINGLE = 0b00010011;
	const static byte EMPTY_MODULE = 0b11111111;
};
