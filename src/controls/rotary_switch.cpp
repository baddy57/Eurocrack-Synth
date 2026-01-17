#include "rotary_switch.h"

RotarySwitch::RotarySwitch(const Address& a, int _positions, const uint_fast16_t* modulePins)
	: positions(_positions)
	, lastValue(0)
{
	switches = new Switch*[positions];
	for (int i = 0; i < positions; ++i) {
		switches[i] = new Switch(a, modulePins[i]);
	}
}

void RotarySwitch::update() {
	if (!switches[lastValue]->wasUpdated())
		return ;
	for (int i = 0; i < positions; ++i) {
		switches[i]->update();
		if (switches[i]->b_read()) {
			lastValue = i;
			return;
		}
	}
}
