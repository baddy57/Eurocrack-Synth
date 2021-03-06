#ifndef __MODULES_VCO_DET_H__
#define __MODULES_VCO_DET_H__

#include "../Module.h"

class VCO_det : public Module {
	private:
		Potentiometer 	
			_coarse_pot0,
			_fine_pot1,
			_freqmodcv_pot2,
			_phasemodcv_pot3,
			_amp_pot4;

		Switch _freq_phase_sw4;
		
		AudioSynthWaveformModulated 
			_sin0,
			_tri0,
			_saw0,
			_sqr0,
			_sin1,
			_tri1,
			_saw1,
			_sqr1;
		AudioAmplifier 	
			_amp0,
			_amp1;
						
		AudioMixer4 
			_sin, 
			_tri, 
			_saw, 
			_sqr;
		Input freqCv;
		Input shapeCv;
		Output sine;
		Output triangle;
		Output sawtooth;
		Output square;
		float 	
			_freq0,
			_freq1,
			_amplitude,
			_freqmodcv,
			_phasemodcv;

	public:
		VCO_det (const Address&);
		void updateValues()override;
};

#endif