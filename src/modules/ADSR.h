#ifndef __MODULES_ADSR_H__
#define __MODULES_ADSR_H__ 1

#include "../Module.h"
#include "../Address.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <vector>
#include <Audio.h>
#include <cstdint>

class ADSR : public Module{
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
	*/
		Potentiometer 	//_del_pot0,
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