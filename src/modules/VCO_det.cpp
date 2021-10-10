#include "VCO_det.h"

#include "Module.h"
#include "Address.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <vector>
#include <Audio.h>
#include <cstdint>
extern ILI9341_t3 tft;

namespace {
	enum _outputs { SIN_OUT, TRI_OUT, SAW_OUT, SQR_OUT};
	enum _inputs {	SHAPE_CV_D=13, XXX_D=14,  
					FREQ_PHASE_CV_D=24, 
					SIN_OUT_D=25, TRI_OUT_D=26, SAW_OUT_D=27, SQR_OUT_D=28, 
					FREQ_PHASE_CV=29, SHAPE_CV, XXX};
};

//ctor
VCO_det :: VCO_det (const Address& a)
	:	Module(a),
		_coarse_pot0	(a, 8), //8
		_fine_pot1		(a, 10), //10
		_freqmodcv_pot2	(a, 11), 
		_phasemodcv_pot3(a, 12), //12
		_amp_pot4		(a, 15),
		
		_freq_phase_sw4	(a, 9) //, //9
		
		//_sin0(AudioSynthWaveformModulated())
		// _tri0(),
		// _saw0(),
		// _sqr0(),
		// _sin1(),
		// _tri1(),
		// _saw1(),
		// _sqr1(),
		// _sin(),
		// _tri(),
		// _saw(),
		// _sqr()

{
	//_sin0 = new AudioSynthWaveformModulated();
	// _tri0 = new AudioSynthWaveformModulated();
	// _saw0 = new AudioSynthWaveformModulated(); 
	// _sqr0 = new AudioSynthWaveformModulated();
	// _sin1 = new AudioSynthWaveformModulated(); 
	// _tri1 = new AudioSynthWaveformModulated(); 
	// _saw1 = new AudioSynthWaveformModulated(); 
	// _sqr1 = new AudioSynthWaveformModulated();
	// _sin  = new AudioMixer4(); 
	// _tri  = new AudioMixer4(); 
	// _saw  = new AudioMixer4(); 
	// _sqr  = new AudioMixer4();
	// _amp0 = new	AudioAmplifier();
	// _amp1 = new	AudioAmplifier();
	
	outputSocket.push_back(new OutputSocket(a, SIN_OUT, SIN_OUT_D, _sin, 0, "SIN"));
	outputSocket.push_back(new OutputSocket(a, TRI_OUT, TRI_OUT_D, _tri, 0, "TRI"));
	outputSocket.push_back(new OutputSocket(a, SAW_OUT, SAW_OUT_D, _saw, 0, "SAW"));
	outputSocket.push_back(new OutputSocket(a, SQR_OUT, SQR_OUT_D, _sqr, 0, "SQR"));
	
	inputSocket.push_back(new InputSocket(a, FREQ_PHASE_CV, FREQ_PHASE_CV_D, _amp0, 0, "FREQ_PHASE_CV"));
	inputSocket.push_back(new InputSocket(a, SHAPE_CV, SHAPE_CV_D, _amp1, 0, "SHAPE_CV"));
	
	//double waveforms to mixers
	internalConns.push_back(new AudioConnection(_sin0,0, _sin,0));
	internalConns.push_back(new AudioConnection(_sin1,0, _sin,1));
	internalConns.push_back(new AudioConnection(_tri0,0, _tri,0));
	internalConns.push_back(new AudioConnection(_tri1,0, _tri,1));
	internalConns.push_back(new AudioConnection(_saw0,0, _saw,0));
	internalConns.push_back(new AudioConnection(_saw1,0, _saw,1));
	internalConns.push_back(new AudioConnection(_sqr0,0, _sqr,0));
	internalConns.push_back(new AudioConnection(_sqr1,0, _sqr,1));
	
	//inputs to waveforms
	internalConns.push_back(new AudioConnection(_amp0,0, _sin0,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _tri0,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _saw0,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _sqr0,0));
	internalConns.push_back(new AudioConnection(_amp1,0, _sin0,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _tri0,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _saw0,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _sqr0,1));
	internalConns.push_back(new AudioConnection(_amp0,0, _sin1,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _tri1,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _saw1,0));
	internalConns.push_back(new AudioConnection(_amp0,0, _sqr1,0));
	internalConns.push_back(new AudioConnection(_amp1,0, _sin1,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _tri1,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _saw1,1));
	internalConns.push_back(new AudioConnection(_amp1,0, _sqr1,1));

	_sin0.begin(1, 200, WAVEFORM_SINE);
	_tri0.begin(1, 200, WAVEFORM_TRIANGLE);
	_saw0.begin(1, 200, WAVEFORM_SAWTOOTH);
	_sqr0.begin(1, 200, WAVEFORM_SQUARE);
	_sin1.begin(1, 200, WAVEFORM_SINE);
	_tri1.begin(1, 200, WAVEFORM_TRIANGLE);
	_saw1.begin(1, 200, WAVEFORM_SAWTOOTH);
	_sqr1.begin(1, 200, WAVEFORM_SQUARE);
}

void
VCO_det :: updateValues() {
	//update frequency
	if(_coarse_pot0.wasUpdated() || _fine_pot1.wasUpdated()){
		float c = _coarse_pot0.i_read();
		float f = _fine_pot1.i_read();
		if(f>256){
			_freq0 = c*c*c/1000000 + (f-256)/10;
			_freq1 = _freq0;
		}
		else{
			_freq0 = c*c*c/1000000;
			_freq1 = c*c*c/1000000 - (256-f)/10;
		}
		
		_sin0.frequency(_freq0);
		_tri0.frequency(_freq0);
		_saw0.frequency(_freq0);
		_sqr0.frequency(_freq0);
		_sin1.frequency(_freq1);
		_tri1.frequency(_freq1);
		_saw1.frequency(_freq1);
		_sqr1.frequency(_freq1);
		// tft.print("freq (Hz)= ");
		// tft.println(_freq);
	}
	
	if(_amp_pot4.wasUpdated()){
		_amplitude = _amp_pot4.f_read();
		_sin0.amplitude(_amplitude);
		_tri0.amplitude(_amplitude);
		_saw0.amplitude(_amplitude);
		_sqr0.amplitude(_amplitude);
		_sin1.amplitude(_amplitude);
		_tri1.amplitude(_amplitude);
		_saw1.amplitude(_amplitude);
		_sqr1.amplitude(_amplitude);
		// tft.print("...b:");
		// tft.println(_amplitude);
	}
	
	_freq_phase_sw4.update();
//		tft.print("uuuuuuuuuu");//updates sw internal variable
	
	if(_freq_phase_sw4.b_read()){ //if switch is set to frequency mod
		if(_freqmodcv_pot2.wasUpdated()){
			_freqmodcv = _freqmodcv_pot2.f_read()*12;
			_sin0.frequencyModulation(_freqmodcv);
			_tri0.frequencyModulation(_freqmodcv);
			_saw0.frequencyModulation(_freqmodcv);
			_sqr0.frequencyModulation(_freqmodcv);
			_sin1.frequencyModulation(_freqmodcv);
			_tri1.frequencyModulation(_freqmodcv);
			_saw1.frequencyModulation(_freqmodcv);
			_sqr1.frequencyModulation(_freqmodcv);
			// tft.print("...c...");
			// tft.println(_freqmodcv);
		}
	}
	else{ //if switch is set to phase modulation
		if(_phasemodcv_pot3.wasUpdated()){
			_phasemodcv = _phasemodcv_pot3.f_read()*9000;
			_sin0.phaseModulation(_phasemodcv);
			_tri0.phaseModulation(_phasemodcv);
			_saw0.phaseModulation(_phasemodcv);
			_sqr0.phaseModulation(_phasemodcv);
			_sin1.phaseModulation(_phasemodcv);
			_tri1.phaseModulation(_phasemodcv);
			_saw1.phaseModulation(_phasemodcv);
			_sqr1.phaseModulation(_phasemodcv);
			
			// tft.print("...d: ");
			// tft.print(_phasemodcv);
		}
	}
}
