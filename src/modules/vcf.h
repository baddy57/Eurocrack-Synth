#pragma once

#include "../core/module.h"

namespace VCF_pins {
	enum inputs { POT0=16, POT1, POT2,
				  SIG_IN_D=25, SIG_IN,
				  FREQ_CV_D, FREQ_CV,
				  HPF_D, BPF_D, LPF_D };
	enum outputs { LPF, BPF, HPF };
}

class VCF : public Module {
private:
	Potentiometer _freq_pot0;
	Potentiometer _res_pot1;
	Potentiometer _oct_pot2;
	ModuleInput in;
	ModuleInput cv;
	ModuleOutput lpf;
	ModuleOutput bpf;
	ModuleOutput hpf;
	AudioFilterStateVariable _filter;

public:
	inline VCF(const Address& a)
		: Module(a)
		, _freq_pot0(a, VCF_pins::POT0, 4700)
		, _res_pot1(a, VCF_pins::POT1, 3200)
		, _oct_pot2(a, VCF_pins::POT2, 3200)
		, in(a, VCF_pins::SIG_IN, VCF_pins::SIG_IN_D, _filter, 0, "VCF_IN")
		, cv(a, VCF_pins::FREQ_CV, VCF_pins::FREQ_CV_D, _filter, 1, "VCF_CV")
		, lpf(a, VCF_pins::LPF, VCF_pins::LPF_D, _filter, 0, "LPF")
		, bpf(a, VCF_pins::BPF, VCF_pins::BPF_D, _filter, 1, "BPF")
		, hpf(a, VCF_pins::HPF, VCF_pins::HPF_D, _filter, 2, "HPF")
	{
		_freq_pot0.setRange(20, 3000, EXP);
		_res_pot1.setRange(0.7, 5, LIN);
		_oct_pot2.setRange(0, 7, LIN);
	}

	inline void updateValues() override {
		if (_freq_pot0.wasUpdated()) {
			float f = _freq_pot0.read();
			_filter.frequency(f);
		}
		if (_res_pot1.wasUpdated()) {
			_filter.resonance(_res_pot1.read());
		}
		if (_oct_pot2.wasUpdated())
			_filter.octaveControl(_oct_pot2.read());
	}
};
