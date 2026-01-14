#pragma once

#include "../core/Module.h"
#include "USBHost_t36.h"
#include <MIDI.h>

extern USBHost usbHost;
extern MIDIDevice midiOnUsbHost;

namespace MidiMono_pins {
	enum inputs { USBSW=24, CHPLUS, CHMINUS, GATE_D=29, CV_D, VEL_D };
	enum outputs { GATE, CV, VEL };
}

class MidiMono : public Module {
private:
	Switch source_sw0;
	Button chplus_btn0, chminus_btn1;

	midi::SerialMIDI<HardwareSerial> serialTransport;
	midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiHardware;

public:
	MidiMono() = delete;

	static AudioSynthWaveformDc _gate;
	static AudioSynthWaveformDc _cv;
	static AudioSynthWaveformDc _vel;
	static uint_fast8_t _channel;

	inline MidiMono(const Address& a)
		: Module(a)
		, source_sw0(a, MidiMono_pins::USBSW)
		, chplus_btn0(a, MidiMono_pins::CHPLUS)
		, chminus_btn1(a, MidiMono_pins::CHMINUS)
		, serialTransport(Serial1)
		, midiHardware(serialTransport)
	{
		using namespace MidiMono_pins;

		outputSockets.push_back(std::make_shared<OutputSocket>(a, GATE, GATE_D, _gate, 0, "midi gate"));
		outputSockets.push_back(std::make_shared<OutputSocket>(a, CV, CV_D, _cv, 0, "midi cv"));
		outputSockets.push_back(std::make_shared<OutputSocket>(a, VEL, VEL_D, _vel, 0, "midi vel"));

		_channel = 1;

		midiHardware.setHandleNoteOn(handleNoteOn);
		midiHardware.setHandleNoteOff(handleNoteOff);
		usbMIDI.setHandleNoteOn(handleNoteOn);
		usbMIDI.setHandleNoteOff(handleNoteOff);
		midiOnUsbHost.setHandleNoteOn(handleNoteOn);
		midiOnUsbHost.setHandleNoteOff(handleNoteOff);
		midiHardware.begin(_channel);
	}

	inline void updateValues() override {
		if (_channel < 16 && chplus_btn0.wasUpdated()) {
			if (chplus_btn0.isPressed()) {
				_channel++;
				tft.print("ch ");
				tft.println(_channel);
			}
		}
		if (_channel > 1 && chminus_btn1.wasUpdated()) {
			if (chminus_btn1.isPressed()) {
				_channel--;
				tft.print("ch ");
				tft.println(_channel);
			}
		}

		source_sw0.update();

		midiHardware.read();
		usbMIDI.read();
		usbHost.Task();
		midiOnUsbHost.read();
	}

	inline static void handleNoteOn(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity) {
		constexpr float DIV120 = 1.0f / 120.0f;
		if (inChannel == _channel && inVelocity != 0) {
			_gate.amplitude(1);
			_cv.amplitude((float(inNote - 120)) * DIV120);
		}
		Serial.println("note on");
	}

	inline static void handleNoteOff(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity) {
		if (inChannel == _channel)
			_gate.amplitude(0);
		Serial.println("note off");
	}
};
