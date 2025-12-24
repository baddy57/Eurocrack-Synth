#include "VCO_det.h"
extern ILI9341_t3 tft;

namespace {
	enum _outputs { SIN_OUT, TRI_OUT, SAW_OUT, SQR_OUT};
	enum _inputs {	SHAPE_CV_D=13, XXX_D=14,  
					FREQ_PHASE_CV_D=24, 
					SIN_OUT_D=25, TRI_OUT_D=26, SAW_OUT_D=27, SQR_OUT_D=28, 
					FREQ_PHASE_CV=29, SHAPE_CV, XXX};
};

//ctor
VCO_det::VCO_det(const Address& a)
	: Module(a)
	, _coarse_pot0(a, 8, 3800.f)
	, _fine_pot1(a, 10, 3200.f)
	, _freqmodcv_pot2(a, 11)
	, _phasemodcv_pot3(a, 12)
	, _amp_pot4(a, 15)
	, _freq_phase_sw4(a, 9)
	, freqCv(a, FREQ_PHASE_CV, FREQ_PHASE_CV_D, _amp0, 0, "FREQ_PHASE_CV")
	, shapeCv(a, SHAPE_CV, SHAPE_CV_D, _amp1, 0, "SHAPE_CV")
	, sine(a, SIN_OUT, SIN_OUT_D, _sin, 0, "SIN")
	, triangle(a, TRI_OUT, TRI_OUT_D, _tri, 0, "TRI")
	, sawtooth(a, SAW_OUT, SAW_OUT_D, _saw, 0, "SAW")
	, square(a, SQR_OUT, SQR_OUT_D, _sqr, 0, "SQR")
{

	//double waveforms to mixers
	internalConns.push_back(new AudioConnection(_sin0, 0, _sin, 0));
	internalConns.push_back(new AudioConnection(_sin1, 0, _sin, 1));
	internalConns.push_back(new AudioConnection(_tri0, 0, _tri, 0));
	internalConns.push_back(new AudioConnection(_tri1, 0, _tri, 1));
	internalConns.push_back(new AudioConnection(_saw0, 0, _saw, 0));
	internalConns.push_back(new AudioConnection(_saw1, 0, _saw, 1));
	internalConns.push_back(new AudioConnection(_sqr0, 0, _sqr, 0));
	internalConns.push_back(new AudioConnection(_sqr1, 0, _sqr, 1));

	//inputs to waveforms
	internalConns.push_back(new AudioConnection(_amp0, 0, _sin0, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _tri0, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _saw0, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _sqr0, 0));
	internalConns.push_back(new AudioConnection(_amp1, 0, _sin0, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _tri0, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _saw0, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _sqr0, 1));
	internalConns.push_back(new AudioConnection(_amp0, 0, _sin1, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _tri1, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _saw1, 0));
	internalConns.push_back(new AudioConnection(_amp0, 0, _sqr1, 0));
	internalConns.push_back(new AudioConnection(_amp1, 0, _sin1, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _tri1, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _saw1, 1));
	internalConns.push_back(new AudioConnection(_amp1, 0, _sqr1, 1));

	_sin0.begin(1, 200, WAVEFORM_SINE);
	_tri0.begin(1, 200, WAVEFORM_TRIANGLE);
	_saw0.begin(1, 200, WAVEFORM_SAWTOOTH);
	_sqr0.begin(1, 200, WAVEFORM_SQUARE);
	_sin1.begin(1, 200, WAVEFORM_SINE);
	_tri1.begin(1, 200, WAVEFORM_TRIANGLE);
	_saw1.begin(1, 200, WAVEFORM_SAWTOOTH);
	_sqr1.begin(1, 200, WAVEFORM_SQUARE);

	_coarse_pot0.setRange(0, 3200, EXP);
	_fine_pot1.setRange(-25, 75, EXP);
	_amp_pot4.setRange(0, 1, EXP);
	_freqmodcv_pot2.setRange(0, 12, LIN);
	_phasemodcv_pot3.setRange(0, 9000, LIN);
}

void
VCO_det :: updateValues() {
	//update frequency
	if(_coarse_pot0.wasUpdated() || _fine_pot1.wasUpdated()){
		float c = _coarse_pot0.read();

		float f = _fine_pot1.read();

		if(f>0){
			_freq0 = c + f;
			_freq1 = _freq0;
		}
		else{
			_freq0 = c;
			_freq1 = c + f;
		}
		
		_sin0.frequency(_freq0);
		_tri0.frequency(_freq0);
		_saw0.frequency(_freq0);
		_sqr0.frequency(_freq0);
		_sin1.frequency(_freq1);
		_tri1.frequency(_freq1);
		_saw1.frequency(_freq1);
		_sqr1.frequency(_freq1);
	}
	
	if(_amp_pot4.wasUpdated()){
		_amplitude = _amp_pot4.read();
		_sin0.amplitude(_amplitude);
		_tri0.amplitude(_amplitude);
		_saw0.amplitude(_amplitude);
		_sqr0.amplitude(_amplitude);
		_sin1.amplitude(_amplitude);
		_tri1.amplitude(_amplitude);
		_saw1.amplitude(_amplitude);
		_sqr1.amplitude(_amplitude);
	}
	
	_freq_phase_sw4.update();
	
	if(_freq_phase_sw4.b_read()){ //if switch is set to frequency mod
		if(_freqmodcv_pot2.wasUpdated()){
			_freqmodcv = _freqmodcv_pot2.read();
			_sin0.frequencyModulation(_freqmodcv);
			_tri0.frequencyModulation(_freqmodcv);
			_saw0.frequencyModulation(_freqmodcv);
			_sqr0.frequencyModulation(_freqmodcv);
			_sin1.frequencyModulation(_freqmodcv);
			_tri1.frequencyModulation(_freqmodcv);
			_saw1.frequencyModulation(_freqmodcv);
			_sqr1.frequencyModulation(_freqmodcv);
		}
	}
	else{ //if switch is set to phase modulation
		if(_phasemodcv_pot3.wasUpdated()){
			_phasemodcv = _phasemodcv_pot3.read();
			_sin0.phaseModulation(_phasemodcv);
			_tri0.phaseModulation(_phasemodcv);
			_saw0.phaseModulation(_phasemodcv);
			_sqr0.phaseModulation(_phasemodcv);
			_sin1.phaseModulation(_phasemodcv);
			_tri1.phaseModulation(_phasemodcv);
			_saw1.phaseModulation(_phasemodcv);
			_sqr1.phaseModulation(_phasemodcv);
		}
	}
}
