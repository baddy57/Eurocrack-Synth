#include "Reverb.h"


namespace {
	enum _inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN=23 };
	enum _outputs {OUT};
};

//ctor
Reverb :: Reverb (const Address& a)
	:	Module(a)
	,	_roomsize_pot0	(a, POT0)
	,	_damping_pot1	(a, POT1)
	,	_bypass (a, BYPASS_SW)
{
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT,OUT_D, _rev, 0, "REVERB OUT"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN,IN_D, _rev, 0, "REVERB IN"));

	_roomsize_pot0.setRange(0, 1, LIN);
	_damping_pot1.setRange(0, 1, LIN);
}

void
Reverb :: updateValues() {
	if (_bypass.wasUpdated()) {
		if (!_bypass.b_read()) {
			_rev.roomsize(0);
			_rev.damping(0);
			return;
		}
		else {
			_rev.roomsize(_roomsize_pot0.read());
			_rev.damping(_damping_pot1.read());
			return;
		}
	}
	if(_roomsize_pot0.wasUpdated())
		_rev.roomsize(_roomsize_pot0.read());
	if(_damping_pot1.wasUpdated())
		_rev.damping(_damping_pot1.read());
}
