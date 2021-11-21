#ifndef __MODULES_VCF_H__
#define __MODULES_VCF_H__

#include "../Module.h"


class VCF : public Module {
	private:
		Potentiometer _freq_pot0;
		Potentiometer _res_pot1;
		Potentiometer _oct_pot2;
		Input in;
		Input cv;
		Output lpf;
		Output bpf;
		Output hpf;
		AudioFilterStateVariable _filter;
		
	public:
		VCF (const Address&);
		void updateValues(); 
		
};

#endif