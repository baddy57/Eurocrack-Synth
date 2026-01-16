#pragma once

#include <cstdint>

#include "address.h"

class Control{
	protected:
		ControlAddress address;
		bool _wasUpdated;
		public:
		uint8_t _pinId;  // Local pin ID within module (0-63)
		Control() = delete;
		Control(const Address& a, uint_fast8_t id) : address(a, id), _wasUpdated(true), _pinId(id), uid(address._id){};
		Control(uint_fast8_t pin) : address(pin),_wasUpdated(true), _pinId(0), uid(address._id){};
		const unsigned int uid;

		/// @brief Get the local pin ID within the module
		uint8_t getPinId() const { return _pinId; }
};
