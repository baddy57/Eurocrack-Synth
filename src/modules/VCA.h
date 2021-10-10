#ifndef __MODULES_VCA_H__
#define __MODULES_VCA_H__ 1

#include "Module.h"
#include "Address.h"
#include <Audio.h>
#include <cstdint>
/* 
#include "CONTROLS_Potentiometer.h"
#include "CONTROLS_Switch.h"
#include "CONTROLS_Button.h" */

class VCA : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection> internalConns;
	*/
	
		AudioEffectMultiply _amp0,
							_amp1;
	
	public:
		VCA () = delete;
		VCA (const Address&);
		void updateValues()override;
};

#endif