#include "MODULES_MidiPoly.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"


#include <vector>
#include <Audio.h>
#include <cstdint>
#include <MIDI.h>

#define DIV127 1/127

namespace {
	enum _inputs {};
	enum _outputSockets {
		NOTE_CH1, GATE_CH1, VELOCITY_CH1,
		NOTE_CH2, GATE_CH2, VELOCITY_CH2,
		NOTE_CH3, GATE_CH3, VELOCITY_CH3,
		NOTE_CH4, GATE_CH4, VELOCITY_CH4,
		NOTE_CH5, GATE_CH5, VELOCITY_CH5,
		};
};

//extern midi::MidiInterface<HardwareSerial> MIDI;

//ctor
MidiPoly :: MidiPoly (const Address& a) 
	:	Module(a),
		MIDI((HardwareSerial&)Serial1)
	//	_gate1(),
	//	_note1()
		{
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, NOTE_CH1,99, _note1, 0));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, GATE_CH1,99, _gate1, 0));
	
//	MIDI_CREATE_DEFAULT_INSTANCE();
	
	MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
	
	MIDI.begin(1);  //channel 1
}

void
MidiPoly :: updateValues() {
	MIDI.read();
}

void 
MidiPoly :: handleNoteOn(	uint8_t inChannel, 
							uint8_t inNote, 
							uint8_t inVelocity) {
	_gate1->amplitude(1);
	_note1->amplitude(((float(inNote))*DIV127));

}

void
MidiPoly :: handleNoteOff(uint8_t inChannel, 
							uint8_t inNote, 
							uint8_t inVelocity) {
	_gate1->amplitude(0);
}
