#pragma once

#include "../core/module.h"
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

	ModuleOutput gateOut;
	ModuleOutput cvOut;
	ModuleOutput velOut;

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
		, gateOut(a, MidiMono_pins::GATE, MidiMono_pins::GATE_D, _gate, 0, "midi gate")
		, cvOut(a, MidiMono_pins::CV, MidiMono_pins::CV_D, _cv, 0, "midi cv")
		, velOut(a, MidiMono_pins::VEL, MidiMono_pins::VEL_D, _vel, 0, "midi vel")
	{
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

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::MIDI_MODULE; }
	
	const char* getModuleName() const override { return "MidiMono"; }

	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		// MidiMono has no analog controls
		digital.push_back(TestControlInfo::createDigital("USB_SW", source_sw0.getModulePin(), TestControlType::SWITCH));
		digital.push_back(TestControlInfo::createDigital("CH+", chplus_btn0.getModulePin(), TestControlType::BUTTON));
		digital.push_back(TestControlInfo::createDigital("CH-", chminus_btn1.getModulePin(), TestControlType::BUTTON));
		// Jack detectors are shown in socket section
	}

	void getTestSockets(std::vector<TestSocketInfo>& sockets) override {
		sockets.push_back(TestSocketInfo::createOutput(gateOut.base));
		sockets.push_back(TestSocketInfo::createOutput(cvOut.base));
		sockets.push_back(TestSocketInfo::createOutput(velOut.base));
	}
	#endif
};
