#pragma once

#include <cstdint>

#include "address.h"

class Control{
	protected:
		ControlAddress address;
		bool _wasUpdated;
	public:
		Control() = delete;
		Control(const Address& a, uint_fast8_t id) : address(a, id), _wasUpdated(true), uid(address._id){};
		Control(uint_fast8_t pin) : address(pin),_wasUpdated(true),uid(address._id){};
		const unsigned int uid;
};
