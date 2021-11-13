#ifndef __MODULES_FX_DISTORTION_AMP_H__
#define __MODULES_FX_DISTORTION_AMP_H__

#include "../../Module.h"

/// @brief implements distortion using bitcrusher
class Distortion_amp : public Module { 
private:
	Potentiometer
		_gain_pot0;
	Switch
		_bypass;

	AudioAmplifier _pre, _post;

public:
	Distortion_amp() = delete;
	Distortion_amp(const Address&);
	void updateValues();
};

#endif