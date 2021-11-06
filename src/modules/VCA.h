#ifndef __MODULES_VCA_H__
#define __MODULES_VCA_H__

#include "../Module.h"

class VCA : public Module {
	private:
	
		AudioEffectMultiply _amp0,
							_amp1;
	
	public:
		VCA (const Address&);
		void updateValues()override;
};

#endif