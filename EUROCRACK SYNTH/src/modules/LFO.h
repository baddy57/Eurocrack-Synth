#ifndef __MODULES_LFO_H__
#define __MODULES_LFO_H__

#include "../Module.h"
#include "../sw_components/GateIn.h"

class LFO : public Module {
	private:
		SelectorMulti waveShapeSel;
		Potentiometer freqPot;
		Potentiometer cvPot;

		Input syncIn;
		Input cvIn;
		Output out;

		GateIn sync;
		int currentWave = -1;
		float freq = 10;

		AudioSynthWaveformModulated wave;
		
	public:
		LFO () = delete;
		LFO (const Address&);
		void updateValues();		
};

#endif