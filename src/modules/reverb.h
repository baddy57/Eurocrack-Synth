#pragma once

#include "../core/module.h"

namespace Reverb_pins {
	enum inputs { BYPASS_SW = 16, POT0, POT1, IN_D, OUT_D, IN=23 };
	enum outputs { OUT };
}

class Reverb : public Module {
private:
	Potentiometer
		_roomsize_pot0,
		_damping_pot1;
	Switch _bypass;
	AudioEffectFreeverb _rev;

	ModuleOutput out;
	ModuleInput in;

public:
	Reverb() = delete;

	inline Reverb(const Address& a)
		: Module(a)
		, _roomsize_pot0(a, Reverb_pins::POT0)
		, _damping_pot1(a, Reverb_pins::POT1)
		, _bypass(a, Reverb_pins::BYPASS_SW)
		, out(a, Reverb_pins::OUT, Reverb_pins::OUT_D, _rev, 0, "REVERB OUT")
		, in(a, Reverb_pins::IN, Reverb_pins::IN_D, _rev, 0, "REVERB IN")
	{
		_roomsize_pot0.setRange(0, 1, LIN);
		_damping_pot1.setRange(0, 1, LIN);
	}

	inline void updateValues() override {
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
		if (_roomsize_pot0.wasUpdated())
			_rev.roomsize(_roomsize_pot0.read());
		if (_damping_pot1.wasUpdated())
			_rev.damping(_damping_pot1.read());
	}
};
