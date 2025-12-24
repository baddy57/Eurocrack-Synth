#pragma once
#include "Switch.h"

/// @brief implements a rotary swith as an array of n 2-position switches
class RotarySwitch
{
public:
	/// @brief constructor
	/// @param a module address
	/// @param positions number of positions 
	/// @param ids array of single pins ids, ordered clockwise
	RotarySwitch(const Address& a, int _positions, const uint* ids);
	void update();
	inline int read() { return lastValue; }
	inline bool wasUpdated() { return read() != lastValue; }
private:
	int positions;
	int lastValue;
	Switch** switches;
};

