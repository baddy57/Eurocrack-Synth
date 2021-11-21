#ifndef __MODULES_ADSR_H__
#define __MODULES_ADSR_H__

#include "../Module.h"

class ADSR : public Module{
	private:
		Potentiometer _att_pot1;
		Potentiometer _sus_pot3;
		Potentiometer _dec_pot4;
		Potentiometer _rel_pot5;
		Input gateIn;
		Output out;

		AudioEffectEnvelope _envelope;
		AudioRecordQueue _gate;//???
		// GateIn _gate
		AudioSynthWaveformDc _signal;
		bool _gateOpen = false;
	public:
		ADSR (const Address&);
		void updateValues();

};

#endif