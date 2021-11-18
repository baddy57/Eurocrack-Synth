#ifndef __MODULES_LFO_H__
#define __MODULES_LFO_H__

#include "../Module.h"

class LFO : public Module {
	private:
		SelectorMulti waveShapeSel;
		Potentiometer freqPot;
		Potentiometer cvPot;
		Switch	rangeSw;

		int currentWave = -1;
		int maxRange = -1;
		float freq = 10;

		AudioSynthWaveformModulated wave;
		
	public:
		LFO () = delete;
		LFO (const Address&);
		void updateValues();		
};

#endif