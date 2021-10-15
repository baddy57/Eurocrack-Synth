#include "DrumMachine.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

//#include <vector>
#include <Audio.h>
#include <cstdint>
#include <MIDI.h>
#define DIV127 1/127

uint_fast8_t DrumMachine::_channel;
std::vector<Voice*> voices;


extern ILI9341_t3 tft;
extern USBHost myusb;
extern MIDIDevice midi1;


namespace {
	enum _inputs {	_B0=8, _B1, _B2, _B3, _B4, _B5, _B6, _B7,
					PAN, GAIN, //16, 17
					S0, S1, S2, S3,
					 CHMINUS, CHPLUS,
					OUT_L_D=29,OUT_R_D,OUT_M_D};
	enum _outputs {	OUT_L, OUT_R, OUT_M};
};

//ctor
DrumMachine :: DrumMachine (const Address& a)
	:	Module(a),
		MIDI((HardwareSerial&)Serial1),
		s0(a, S0), s1(a, S0), s2(a, S2), s3(a, S3),
		chplus(a, CHPLUS),
		chminus(a, CHMINUS),
		pan_pot(a, PAN),
		gain_pot(a, GAIN)
{
	for(uint_fast8_t i=0; i<9; ++i)
		mxr[i] = new AudioMixer4();
	
	
	voices.push_back(new Voice(a, _B0));
	voices.push_back(new Voice(a, _B1));
	voices.push_back(new Voice(a, _B2));
	voices.push_back(new Voice(a, _B3));
	voices.push_back(new Voice(a, _B4));
	voices.push_back(new Voice(a, _B5));
	voices.push_back(new Voice(a, _B6));
	voices.push_back(new Voice(a, _B7));
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_L, OUT_L_D, *mxr[6], 0, "drum left"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_R, OUT_R_D, *mxr[7], 0, "drum right"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_M, OUT_M_D, *mxr[8], 0, "drum mono"));
	
	// to internal mixer4
	for(uint_fast8_t i=0; i<4; ++i)
		for(uint_fast8_t j=0; j<3; ++j){
			internalConns.push_back(new AudioConnection(voices[i]->wavetable,0, *(mxr[j%3*2]),i));
			internalConns.push_back(new AudioConnection(voices[i+4]->wavetable,0, *(mxr[j%3*2+1]),i+4));
		}
		/* 
		internalConns.push_back(new AudioConnection(voices[i]->wavetable,0, mxr[0],i));
		internalConns.push_back(new AudioConnection(voices[i]->wavetable,0, mxr[2],i));
		internalConns.push_back(new AudioConnection(voices[i]->wavetable,0, mxr[4],i));
		internalConns.push_back(new AudioConnection(voices[i+4]->wavetable,0, mxr[1],i+4));
		internalConns.push_back(new AudioConnection(voices[i+4]->wavetable,0, mxr[3],i+4));
		internalConns.push_back(new AudioConnection(voices[i+4]->wavetable,0, mxr[5],i+4)); */
	//mixer4 to mixer8
	for(uint_fast8_t i=0; i<6; ++i)
		internalConns.push_back(new AudioConnection(*(mxr[i]),0, *(mxr[i/2+6]),i%2));
	/*
	internalConns.push_back(new AudioConnection(mxr[0],0, mxr[6],0));//left
	internalConns.push_back(new AudioConnection(mxr[1],0, mxr[6],1));
	internalConns.push_back(new AudioConnection(mxr[2],0, mxr[7],0));//right
	internalConns.push_back(new AudioConnection(mxr[3],0, mxr[7],1));
	internalConns.push_back(new AudioConnection(mxr[4],0, mxr[8],0));//mono
	internalConns.push_back(new AudioConnection(mxr[5],0, mxr[8],1)); */
	
	
	_channel=2;
	
	MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
	usbMIDI.setHandleNoteOn(handleNoteOn);
	usbMIDI.setHandleNoteOff(handleNoteOff);
	midi1.setHandleNoteOn(handleNoteOn);
    midi1.setHandleNoteOff(handleNoteOff);
	
	MIDI.begin(_channel);
	
}

void
DrumMachine :: updateValues() {
	if (_channel<16 && chplus.wasUpdated()){
		if (chplus.isPressed()){
			_channel++;
			tft.print("ch ");
			tft.println(_channel);
		}
	}
	if (_channel>1 && chminus.wasUpdated()){
		if (chminus.isPressed()){
			_channel--;
			tft.print("ch ");
			tft.println(_channel);
		}
	}
	//knobs and buttons
	for(uint_fast8_t i=0; i<8; ++i){
		uint_fast8_t x=1;
		if(i<4)x=0;
		if(voices[i]->b.isPressed()){ //while i'm pressing one of b0,b7
			if(pan_pot.wasUpdated()){
				float g=voices[i]->gain;
				float r=pan_pot.f_read();
				mxr[x]->gain(i%4, g*(1.0-r));
				mxr[x+2]->gain(i%4, g*r);
			}
			if(gain_pot.wasUpdated()){
				voices[i]->setGain(gain_pot.f_read());
				mxr[x+4]->gain(i%4, voices[i]->gain);
			}
			//bank --
			if(voices[i]->bank>0 						&& s0.wasUpdated() && s0.isPressed()){
				voices[i]->prevBank();
			}
			//bank ++
			if(voices[i]->bank<banks-1 					&& s1.wasUpdated() && s1.isPressed()){
				voices[i]->nextBank();
			}
			//sample--
			if(voices[i]->sample>0 						&& s2.wasUpdated() && s2.isPressed()){
				voices[i]->prevSample();
				handleNoteOn(_channel, i+44, 127);
			}
			//sample++
			if(voices[i]->sample<voices[i]->last_sample && s3.wasUpdated() && s3.isPressed()){
				voices[i]->nextSample();
				handleNoteOn(_channel, i+44, 127);
			}
		}
	}
	myusb.Task();
	midi1.read();
}

void 
DrumMachine :: handleNoteOn(	uint8_t inChannel, 
								uint8_t inNote, 
								uint8_t inVelocity) {
	if(inChannel==_channel && inVelocity!=0){
		if(inNote>=44&&inNote<=51){//mpkmini pads
			voices[inNote-44]->wavetable.playNote(voices[inNote-44]->sample, inVelocity);
		//	tft.print("note on");
		}
	}
}

void
DrumMachine :: handleNoteOff(	uint8_t inChannel, 
								uint8_t inNote, 
								uint8_t inVelocity) {
	if(inChannel==_channel)
		if(inNote>=44&&inNote<=51)//mpkmini pads
			voices[inNote-44]->wavetable.stop();
//	tft.println("note off");
}

/*
48	49	50	51		//on mpk mini
44	45	46	47

4	5	6	7		//on module
0	1	2	3
*/