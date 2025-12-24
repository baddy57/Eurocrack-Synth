#include "Distortion_bc.h"

namespace {
	enum _inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN = 23 };
	enum _outputs { OUT };
};

//ctor
Distortion_bc::Distortion_bc(const Address& a)
	: Module(a)
	, _bits_pot0(a, POT0, 4700)
	, _sampleRate_pot1(a, POT1, 4700)
	, _bypass(a, BYPASS_SW)
{

	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT, OUT_D, _dist, 0, "Distortion_bc OUT"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN, IN_D, _dist, 0, "Distortion_bc IN"));

	_bits_pot0.setRange(16, 1, LIN);
	_sampleRate_pot1.setRange(44100, 1, EXP); 
}

void
Distortion_bc::updateValues() {
	if (_bypass.wasUpdated()) {
		if (!_bypass.b_read()) {
			_dist.bits(16);
			_dist.sampleRate(44100);
			return;
		}
	}

	if (_bits_pot0.wasUpdated()) {
		uint8_t val = _bits_pot0.read();
		_dist.bits(val);
	}
	if (_sampleRate_pot1.wasUpdated()) {
		float val = _sampleRate_pot1.read();
		_dist.sampleRate(val);
	}
}
