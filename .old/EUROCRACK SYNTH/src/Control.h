#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <cstdint>

#include "Address.h"

class Control{
	protected:
		ControlAddress address;
		bool _wasUpdated;
	public:
		Control() = delete;
		Control(const Address& a, uint_fast8_t id) : address(/* new ControlAddress( */a, id), _wasUpdated(true), uid(address._id){};
		Control(uint_fast8_t pin) : address(pin),_wasUpdated(true),uid(address._id){};
		const unsigned int uid;
};

#endif