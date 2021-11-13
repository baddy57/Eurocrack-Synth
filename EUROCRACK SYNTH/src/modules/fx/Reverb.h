#ifndef __MODULES_REVERB_H__
#define __MODULES_REVERB_H__

#include "../../Module.h"

class Reverb : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection*> internalConns;//if any;
	*/
		Potentiometer 	
			_roomsize_pot0, 
			_damping_pot1;
		Switch
			_bypass;


		AudioEffectFreeverb _rev;
		
	public:
		Reverb () = delete;
		Reverb (const Address&);
		void updateValues();		
};

#endif