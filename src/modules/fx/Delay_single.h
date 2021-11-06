#pragma once
#include <Module.h>

/// @brief 
class Delay_single : public Module {
protected:
	Potentiometer
		pot0;
	
	AudioEffectDelay delay;

public:
	Delay_single(const Address&);
	void updateValues()override;
};