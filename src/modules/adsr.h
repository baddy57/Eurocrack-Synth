#pragma once

#include "../core/module.h"

namespace ADSR_pins {
	enum inputs {
		ATT_CV=8, ATT_CV_D,
		DEC_CV, DEC_CV_D,
		ATT, _DEC,
		GATE,
		SUS=24, _REL,
		SUS_CV, SUS_CV_D,
		REL_CV, REL_CV_D,
		SIGNAL_OUT_D, GATE_D
	};
	enum outputs { SIGNAL_OUT };
}

class ADSR : public Module {
private:
	Potentiometer _att_pot1;
	Potentiometer _sus_pot3;
	Potentiometer _dec_pot4;
	Potentiometer _rel_pot5;
	ModuleInput gateIn;
	ModuleOutput out;

	AudioEffectEnvelope _envelope;
	AudioRecordQueue _gate;
	AudioSynthWaveformDc _signal;
	bool _gateOpen = false;

public:
	inline ADSR(const Address& a)
		: Module(a)
		, _att_pot1(a, ADSR_pins::ATT, 4700)
		, _sus_pot3(a, ADSR_pins::SUS, 4700)
		, _dec_pot4(a, ADSR_pins::_DEC, 4700)
		, _rel_pot5(a, ADSR_pins::_REL, 4700)
		, gateIn(a, ADSR_pins::GATE, ADSR_pins::GATE_D, _gate, 0, "ADSR_GATE")
		, out(a, ADSR_pins::SIGNAL_OUT, ADSR_pins::SIGNAL_OUT_D, _envelope, 0, "ADSR_OUT")
	{
		_signal.amplitude(1);
		_envelope.attack(100);
		_envelope.release(2000);
		_envelope.delay(0);
		_envelope.hold(0);

		internalConns.push_back(new AudioConnection(_signal, 0, _envelope, 0));

		_gate.begin();

		_att_pot1.setRange(0, 10000, EXP);
		_dec_pot4.setRange(0, 10000, EXP);
		_sus_pot3.setRange(0, 1, EXP);
		_rel_pot5.setRange(0, 10000, EXP);
	}

	inline void updateValues() override {
		if (_att_pot1.wasUpdated())
			_envelope.attack(_att_pot1.read());

		if (_sus_pot3.wasUpdated())
			_envelope.sustain(_sus_pot3.read());

		if (_dec_pot4.wasUpdated())
			_envelope.decay(_dec_pot4.read());

		if (_rel_pot5.wasUpdated())
			_envelope.release(_rel_pot5.read());

		if (_gate.available() > 0) {
			int16_t xxx[128];
			memcpy(xxx, _gate.readBuffer(), 256);
			_gate.freeBuffer();
			bool newGateReading = (xxx[0] > 0);
			if (newGateReading != _gateOpen) {
				_gateOpen = newGateReading;
				if (_gateOpen)
					_envelope.noteOn();
				else
					_envelope.noteOff();
			}
			_gate.clear();
		}
	}
};
