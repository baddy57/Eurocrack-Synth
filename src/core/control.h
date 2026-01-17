#pragma once

#include <cstdint>

#include "address.h"

class Control{
	protected:
		ControlAddress address;

		bool _wasUpdated;

	public:
		uint8_t _modulePin;  // Local pin ID within module (0-63)

		Control() = delete;

		Control(const Address& moduleAddress, uint_fast8_t modulePin) 
			: address(moduleAddress, modulePin), _wasUpdated(true), _modulePin(modulePin){};

		Control(uint_fast8_t arduinoPin) 
			: address(arduinoPin),_wasUpdated(true), _modulePin(0){};

		unsigned int getId() const { return address.getId(); }

		/// @brief Get the local pin ID within the module
		uint8_t getModulePin() const { return _modulePin; }
};
