#include "ADSR.h"

namespace {
	enum _inputs {	
		ATT_CV=8, ATT_CV_D, 
		DEC_CV, DEC_CV_D,
		ATT, _DEC,
		GATE,
		SUS=24, _REL,
		SUS_CV, SUS_CV_D, 
		REL_CV, REL_CV_D,
		SIGNAL_OUT_D, GATE_D 
	};
	enum _outputs {SIGNAL_OUT};
};

//ctor
ADSR :: ADSR (const Address& a)
:	Module(a)
,	_att_pot1(a, ATT, 4700)
,	_sus_pot3(a, SUS, 4700)
,	_dec_pot4(a, _DEC, 4700)
,	_rel_pot5(a, _REL, 4700)
,	gateIn(a, GATE, GATE_D, _gate, 0, "ADSR_GATE")
,	out(a, SIGNAL_OUT, SIGNAL_OUT_D, _envelope, 0, "ADSR_OUT")
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

void
ADSR :: updateValues() {
	if(_att_pot1.wasUpdated())
		_envelope.attack(_att_pot1.read());

	if(_sus_pot3.wasUpdated())
		_envelope.sustain(_sus_pot3.read());

	if(_dec_pot4.wasUpdated())
		_envelope.decay(_dec_pot4.read());

	if(_rel_pot5.wasUpdated())
		_envelope.release(_rel_pot5.read());
	
	if(_gate.available()>0){
		int16_t xxx[128];
		memcpy(xxx, _gate.readBuffer(), 256);
		_gate.freeBuffer();
		bool newGateReading = (xxx[0]>0);
		/* if (newGateReading)
			_envelope.noteOn();
		if(!newGateReading && _envelope.isActive())
			_envelope.noteOff(); */
		if(newGateReading != _gateOpen) {
			_gateOpen = newGateReading;
			if(_gateOpen) 
				_envelope.noteOn();
			else 
				_envelope.noteOff();
		}
		_gate.clear();
	}
}
