#include "ADSR.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <vector>
#include <Audio.h>
#include <cstdint>

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
:	//init list
	Module(a),
//	_del_pot0(a, 0),
	_att_pot1(a, ATT),
//	_hol_pot2(a, 2),
	_sus_pot3(a, SUS),
	_dec_pot4(a, _DEC),
	_rel_pot5(a, _REL)//,
{
	// _envelope = new AudioEffectEnvelope();
	// _gate     = new AudioRecordQueue();
	// _signal   = new AudioSynthWaveformDc();
	
	_signal.amplitude(1);
	_envelope.attack(100);
	_envelope.release(2000);
	_envelope.delay(0);
	_envelope.hold(0);

	inputSockets.push_back(std::make_shared<InputSocket>(a, GATE, GATE_D, _gate, 0, "ADSR_GATE"));
	//sharedInputSockets.push_back(std::make_shared<InputSocket>(a, GATE, GATE_D, _gate, 0, "ADSR_GATE"));
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, SIGNAL_OUT, SIGNAL_OUT_D, _envelope, 0, "ADSR_OUT"));
	//sharedOutputSockets.push_back(std::make_shared<OutputSocket>(a, SIGNAL_OUT, SIGNAL_OUT_D, _envelope, 0, "ADSR_OUT"));
	
	internalConns.push_back(new AudioConnection(_signal, 0, _envelope, 0));
	_gate.begin();
}

void
ADSR :: updateValues() {
	// if(_del_pot0.wasUpdated())
		// _envelope.delay(_del_pot0.f_read()*1000);
	if(_att_pot1.wasUpdated())
		_envelope.attack(_att_pot1.half_f_read()*1000);
	// if(_hol_pot2.wasUpdated())
		// _envelope.hold(_hol_pot2.f_read()*10000);
	if(_sus_pot3.wasUpdated())
		_envelope.sustain(_sus_pot3.half_f_read());
	if(_dec_pot4.wasUpdated())
		_envelope.decay(_dec_pot4.half_f_read()*1000);
	if(_rel_pot5.wasUpdated())
		_envelope.release(_rel_pot5.half_f_read()*1000);
	
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
