#pragma once

#include "../core/Module.h"

class VCA : public Module {
	private:
	
		AudioEffectMultiply _amp0,
							_amp1;
	
	public:
		VCA (const Address&);
		void updateValues()override;
};
