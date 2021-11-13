#pragma once
#include "../../Module.h"

/// @brief 
class Granular : public Module {
protected:
	Potentiometer
		pot0,
		pot1,
		pot2;
	Switch
		sw0,
		sw1;
	Button
		btn0,
		btn1,
		btn2;
	
	AudioEffectGranular granular;

public:
	Granular(const Address&);
	void updateValues()override;
};