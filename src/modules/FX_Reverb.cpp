#include "FX_Reverb.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

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
}

void
Reverb :: updateValues() {
	if (_bypass.wasUpdated()) {
		if (_bypass.b_read()) {
			_rev.roomsize(0);
			_rev.damping(0);
			return;
		}
		else {
			_rev.roomsize(_roomsize_pot0.f_read());
			_rev.damping(_damping_pot1.f_read());
			return;
		}
	}
	if(_roomsize_pot0.wasUpdated())
		_rev.roomsize(_roomsize_pot0.f_read());
	if(_damping_pot1.wasUpdated())
		_rev.damping(_damping_pot1.f_read());
}
