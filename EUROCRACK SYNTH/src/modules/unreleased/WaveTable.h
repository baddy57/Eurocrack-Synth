#ifndef __MODULES_WaveTable_H__
#define __MODULES_WaveTable_H__

#include "../Module.h"

class WaveTable : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection*> internalConns;//if any;
	*/
		Button _b0, _b1, _b2, _b3;
		
		GateIn gateIn;
		AudioSynthWavetable wt0, wt1, wt2, wt3;
		AudioMixer4 masterMixer;
		std::vector<instrument_data*> instruments;
		
		//Button btn0, btn1;
		
		//internal variables
		
		//<streamtype> im0, im1;
		
	public:
		WaveTable () = delete;
		WaveTable (const Address&);
		void updateValues(); //(uint_fast8_t priority) ?
		
};

#endif