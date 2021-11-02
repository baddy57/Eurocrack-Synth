#include "Distortion_amp.h"

namespace {
	enum _inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN = 23 };
	enum _outputs { OUT };
};

//ctor
Distortion_amp::Distortion_amp(const Address& a)
	: Module(a)
	, _gain_pot0(a, POT0, 4700)
	//, _sampleRate_pot1(a, POT1, 4700)
	, _bypass(a, BYPASS_SW)
{

	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT, OUT_D, _post, 0, "Distortion_amp OUT"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN, IN_D, _pre, 0, "Distortion_amp IN"));

	internalConns.push_back(new AudioConnection(_pre, 0, _post, 0));
}

void
Distortion_amp::updateValues() {
	if (_bypass.wasUpdated()) {
		if (!_bypass.b_read()) {
			_pre.gain(1.f);
			_post.gain(1.f);
			return;
		}
	}

	if (_gain_pot0.wasUpdated()) {
		float val = _gain_pot0.f_read();

		_pre.gain(val*100);
		_post.gain(0.1*val);
	}
	
}
