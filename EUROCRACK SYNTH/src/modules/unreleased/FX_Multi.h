#ifndef __MODULES_FX_Multi_H__
#define __MODULES_FX_Multi_H__ 1

#include "../Module.h"
#include "../Address.h"
#include <Audio.h>
#include <cstdint>
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

class FX_Multi : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection*> internalConns;//if any;
	*/
		Potentiometer 	_pot0, _pot1, _pot2;
		
		Switch 	_sw[6];
		
		//Button btn0, btn1;
		
		//internal variables
		
		//<streamtype> im0, im1;
		
	public:
		FX_Multi () = delete;
		FX_Multi (const Address&);
		void updateValues(); //(uint_fast8_t priority) ?
		
};

#endif