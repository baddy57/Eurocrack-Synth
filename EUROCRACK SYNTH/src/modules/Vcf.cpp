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
	,	_freq_pot0	(a, POT0, 4700)
	,	_res_pot1	(a, POT1, 3200)
	,	_oct_pot2	(a, POT2, 3200)
	,	in(a, SIG_IN, SIG_IN_D, _filter, 0, "VCF_IN")
	,	cv(a, FREQ_CV, FREQ_CV_D, _filter, 1, "VCF_CV")
	,	lpf(a, LPF, LPF_D, _filter, 0, "LPF")
	,	bpf(a, BPF, BPF_D, _filter, 1, "BPF")
	,	hpf(a, HPF, HPF_D, _filter, 2, "HPF")
{
	_freq_pot0.setRange(0, 20000, EXP);
	_res_pot1.setRange(0.7, 5, LIN);
	_oct_pot2.setRange(0, 7, LIN);
}

void
VCF :: updateValues() {
	if (_freq_pot0.wasUpdated()) {
		float f = _freq_pot0.read();
		_filter.frequency(f);
	}
	if (_res_pot1.wasUpdated()) {
		_filter.resonance(_res_pot1.read());
	}
	if(_oct_pot2.wasUpdated())
		_filter.octaveControl(_oct_pot2.read());
}
