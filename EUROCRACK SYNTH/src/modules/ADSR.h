#ifndef __MODULES_ADSR_H__
#define __MODULES_ADSR_H__

#include "../Module.h"

class ADSR : public Module{
	private:
		Potentiometer 	
			//_del_pot0,
			_att_pot1,
			//_hol_pot2,
			_sus_pot3,
			_dec_pot4,
			_rel_pot5;

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