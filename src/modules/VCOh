#ifndef __MODULES_VCO_H__
#define __MODULES_VCO_H__

#include "Module.h"

class VCO : public Module {
	private:
		Potentiometer 	_coarse_pot0,
						_fine_pot1,
						_freqmodcv_pot2,
						_phasemodcv_pot3,
						_amp_pot4;
		Switch _freq_phase_sw4;
		
		AudioSynthWaveformModulated *_sin,
									*_tri,
									*_saw,
									*_sqr;
		AudioAmplifier 	*_amp0,
						*_amp1;

		float 	_freq,
				_amplitude,
				_freqmodcv,
				_phasemodcv;
	public:
		VCO () = delete;
		VCO (const Address&);
		void updateValues()override;
};

#endif