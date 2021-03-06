#include "VCO.h"

extern ILI9341_t3 tft;

namespace {
	enum _outputs { SIN_OUT, TRI_OUT, SAW_OUT, SQR_OUT};
	enum _inputs {	SHAPE_CV_D=13, XXX_D=14,  
					FREQ_PHASE_CV_D=24, 
					SIN_OUT_D=25, TRI_OUT_D=26, SAW_OUT_D=27, SQR_OUT_D=28, 
					FREQ_PHASE_CV=29, SHAPE_CV, XXX};
};

//ctor
VCO :: VCO (const Address& a)
	:	Module(a),
		_coarse_pot0	(a, 8), //8
		_fine_pot1		(a, 10), //10
		_freqmodcv_pot2	(a, 11), 
		_phasemodcv_pot3(a, 12), //12
		_amp_pot4		(a, 15),
		
		_freq_phase_sw4	(a, 9), //9
		
		_sin(), 
		_tri(), 
		_saw(), 
		_sqr(){
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, SIN_OUT, SIN_OUT_D, _sin, 0, "SIN"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, TRI_OUT, TRI_OUT_D, _tri, 0, "TRI"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, SAW_OUT, SAW_OUT_D, _saw, 0, "SAW"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, SQR_OUT, SQR_OUT_D, _sqr, 0, "SQR"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, FREQ_PHASE_CV, FREQ_PHASE_CV_D, _amp0, 0, "FREQ_PHASE_CV"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, SHAPE_CV, SHAPE_CV_D, _amp1, 0, "SHAPE_CV"));
	
	internalConns.push_back(new AudioConnection(*_amp0,0, *_sin,0));
	internalConns.push_back(new AudioConnection(*_amp0,0, *_tri,0));
	internalConns.push_back(new AudioConnection(*_amp0,0, *_saw,0));
	internalConns.push_back(new AudioConnection(*_amp0,0, *_sqr,0));
	internalConns.push_back(new AudioConnection(*_amp1,0, *_sin,1));
	internalConns.push_back(new AudioConnection(*_amp1,0, *_tri,1));
	internalConns.push_back(new AudioConnection(*_amp1,0, *_saw,1));
	internalConns.push_back(new AudioConnection(*_amp1,0, *_sqr,1));

	_sin->begin(1, 200, WAVEFORM_SINE);
	_tri->begin(1, 200, WAVEFORM_TRIANGLE);
	_saw->begin(1, 200, WAVEFORM_SAWTOOTH);
	_sqr->begin(1, 200, WAVEFORM_SQUARE);
}

void
VCO :: updateValues() {
	//update frequency
	if(_coarse_pot0.wasUpdated() || _fine_pot1.wasUpdated()){
		float c = _coarse_pot0.i_read();
		_freq = c*c*c/1000000 + (_fine_pot1.i_read())/10;
		_sin->frequency(_freq);
		_tri->frequency(_freq);
		_saw->frequency(_freq);
		_sqr->frequency(_freq);
		// tft.print("freq (Hz)= ");
		// tft.println(_freq);
	}
	
	if(_amp_pot4.wasUpdated()){
		_amplitude = _amp_pot4.f_read();
		_sin->amplitude(_amplitude);
		_tri->amplitude(_amplitude);
		_saw->amplitude(_amplitude);
		_sqr->amplitude(_amplitude);
		// tft.print("...b:");
		// tft.println(_amplitude);
	}
	
	_freq_phase_sw4.update();
//		tft.print("uuuuuuuuuu");//updates sw internal variable
	
	if(_freq_phase_sw4.b_read()){ //if switch is set to frequency mod
		if(_freqmodcv_pot2.wasUpdated()){
			_freqmodcv = _freqmodcv_pot2.f_read()*12;
			_sin->frequencyModulation(_freqmodcv);
			_tri->frequencyModulation(_freqmodcv);
			_saw->frequencyModulation(_freqmodcv);
			_sqr->frequencyModulation(_freqmodcv);
			// tft.print("...c...");
			// tft.println(_freqmodcv);
		}
	}
	else{ //if switch is set to phase modulation
		if(_phasemodcv_pot3.wasUpdated()){
			_phasemodcv = _phasemodcv_pot3.f_read()*9000;
			_sin->phaseModulation(_phasemodcv);
			_tri->phaseModulation(_phasemodcv);
			_saw->phaseModulation(_phasemodcv);
			_sqr->phaseModulation(_phasemodcv);
			
			// tft.print("...d: ");
			// tft.print(_phasemodcv);
		}
	}
}
