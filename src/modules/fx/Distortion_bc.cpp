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
}

void
Distortion_bc::updateValues() {
	if (_bypass.wasUpdated()) {
		if (!_bypass.b_read()) {
			_dist.bits(16);
			_dist.sampleRate(44100);
			return;
		}
		/*else {
			_rev.roomsize(_bits_pot0.f_read());
			_rev.damping(_sampleRate_pot1.f_read());
			return;
		}*/
	}

	if (_bits_pot0.wasUpdated()) {
		uint8_t val = _bits_pot0.i_read();
		_dist.bits(16-val/8);
	}
	if (_sampleRate_pot1.wasUpdated()) {
		float val = _sampleRate_pot1.f_read();
		_dist.sampleRate(44100-val*44100);
	}
}
