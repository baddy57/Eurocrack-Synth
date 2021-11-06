#ifndef __MODULES_VCF_H__
#define __MODULES_VCF_H__

#include "../Module.h"


class VCF : public Module {
	private:
		Potentiometer _freq_pot0, _res_pot1, _oct_pot2;
	
		AudioFilterStateVariable _filter;
		
	public:
		VCF (const Address&);
		void updateValues(); 
		
};

#endif