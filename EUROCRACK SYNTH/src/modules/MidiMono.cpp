#include "MidiMono.h"

extern USBHost myusb;
extern MIDIDevice midi1;

#define DIV127 1/127
#define DIV120 1/120

AudioSynthWaveformDc MidiMono::_gate;
AudioSynthWaveformDc MidiMono::_cv;
AudioSynthWaveformDc MidiMono::_vel;
uint_fast8_t MidiMono::_channel;


namespace {
	enum _inputs {	USBSW=24, 
					CHPLUS, CHMINUS, 
					GATE_D=29, CV_D, VEL_D};
	enum _outputs {GATE, CV, VEL};
};

//extern midi::MidiInterface<HardwareSerial> MIDI;

//ctor
MidiMono :: MidiMono (const Address& a) 
	:	Module(a),
		source_sw0(a, USBSW),
		chplus_btn0(a, CHPLUS),
		chminus_btn1(a, CHMINUS),
		MIDI((HardwareSerial&)Serial1)
	/*,
		_gate(),
		_cv(),
		_vel()*/
{
	
	
	// _gate = new AudioSynthWaveformDc();
	// _cv   =	new AudioSynthWaveformDc();
	// _vel  = new AudioSynthWaveformDc();
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, GATE, GATE_D, _gate, 0, "midi gate"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, CV, CV_D, _cv, 0, "midi cv"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, VEL, VEL_D, _vel, 0, "midi vel"));
	
//	MIDI_CREATE_DEFAULT_INSTANCE();
	
	_channel=1;
	
	MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
	usbMIDI.setHandleNoteOn(handleNoteOn);
	usbMIDI.setHandleNoteOff(handleNoteOff);
	midi1.setHandleNoteOn(handleNoteOn);
    midi1.setHandleNoteOff(handleNoteOff);
	
	MIDI.begin(_channel);
	
}

void
MidiMono :: updateValues() {
	if (_channel<16 && chplus_btn0.wasUpdated()){
		if (chplus_btn0.isPressed()){
			_channel++;
			tft.print("ch ");
			tft.println(_channel);
		}
	}
	if (_channel>1 && chminus_btn1.wasUpdated()){
		if (chminus_btn1.isPressed()){
			_channel--;
			tft.print("ch ");
			tft.println(_channel);
		}
	}
	
	
	source_sw0.update();
	if (source_sw0.b_read())
		MIDI.read();
	else{
		usbMIDI.read();
		myusb.Task();
		midi1.read();
	}
}

void 
MidiMono :: handleNoteOn(	uint8_t inChannel, 
							uint8_t inNote, 
							uint8_t inVelocity) {
	if(inChannel==_channel && inVelocity!=0){
		_gate.amplitude(1);
		_cv.amplitude(((float(inNote-120))*DIV120));
	}
	// tft.print("note on");
}

void
MidiMono :: handleNoteOff(uint8_t inChannel, 
							uint8_t inNote, 
							uint8_t inVelocity) {
	if(inChannel==_channel)							
		_gate.amplitude(0);
	// tft.println("note off");
}
