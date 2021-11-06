#include "Vcf.h"

namespace {
	enum _inputs {	POT0=16, POT1, POT2,
					SIG_IN_D=25, SIG_IN,
					FREQ_CV_D, FREQ_CV,
					HPF_D,BPF_D,LPF_D};
	enum _outputs {LPF, BPF, HPF};
};

//ctor
VCF :: VCF (const Address& a) 
	:	Module(a)
	,	_freq_pot0	(a, POT0, 3200)
	,	_res_pot1	(a, POT1, 3200)
	,	_oct_pot2	(a, POT2, 3200)
{
	inputSockets.push_back(std::make_shared<InputSocket>(a, SIG_IN, SIG_IN_D, _filter, 0, "VCF_IN"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, FREQ_CV, FREQ_CV_D, _filter, 1, "VCF_CV"));
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, LPF, LPF_D, _filter, 0, "LPF"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, BPF, BPF_D, _filter, 1, "BPF"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, HPF, HPF_D, _filter, 2, "HPF"));
}

void
VCF :: updateValues() {
	if(_freq_pot0.wasUpdated()){
		float c = _freq_pot0.i_read();
		_filter.frequency(c*c*c/100000);
	}
	if (_res_pot1.wasUpdated()) {
		float r = _res_pot1.f_read() * 4.3 + 0.7;
		_filter.resonance(r);
	}
	if(_oct_pot2.wasUpdated())
		_filter.octaveControl(_oct_pot2.f_read()*7);
}
