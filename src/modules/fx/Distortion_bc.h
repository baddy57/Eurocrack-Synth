#ifndef __MODULES_FX_DISTORTION_BC_H__
#define __MODULES_FX_DISTORTION_BC_H__

#include "../../Module.h"

/// @brief implements distortion using bitcrusher
class Distortion_bc : public Module { 
private:
	Potentiometer
		_bits_pot0,
		_sampleRate_pot1;
	Switch
		_bypass;

	AudioEffectBitcrusher _dist;

public:
	Distortion_bc() = delete;
	Distortion_bc(const Address&);
	void updateValues();
};

#endif