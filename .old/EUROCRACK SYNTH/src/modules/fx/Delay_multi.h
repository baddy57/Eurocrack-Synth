#pragma once
#include "../../Module.h"

/// @brief 
class Delay_multi : public Module {
protected:
	Potentiometer
		pot0, pot1;
	
	AudioEffectDelay delay;
	AudioMixer4 intmixer0, intmixer1, outmix;

public:
	Delay_multi(const Address&);
	void updateValues()override;
};