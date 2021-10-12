#ifndef __MODULES_VCF_H__
#define __MODULES_VCF_H__ 1

#include "../Module.h"
#include "../Address.h"
#include <Audio.h>
#include <cstdint>
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

class VCF : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
	*/
		Potentiometer _freq_pot0, _res_pot1, _oct_pot2;
	
		AudioFilterStateVariable _filter;
		
	public:
		VCF () = delete;
		VCF (const Address&);
		void updateValues(); 
		
};

#endif