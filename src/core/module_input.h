#pragma once

#include <memory>
#include <cstdint>
#include <Audio.h>

#include "address.h"
#include "../controls/input_socket.h"
#include "../services/patch_cable_manager.h"

struct ModuleInput {
	inline  ModuleInput(const Address& a,
		uint_fast8_t modulePin,
		uint_fast8_t detectormodulePin,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono in")
	{
		base = std::make_shared<InputSocket>(a, modulePin, detectormodulePin, as, port, name);
		PatchCableManager::registerInput(base);
	}

	inline ModuleInput(const Address& a,
		uint_fast8_t modulePin,
		uint_fast8_t detectormodulePin,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly in")
	{
		base = std::make_shared<InputSocket>(a, modulePin, detectormodulePin, as0, as1, as2, as3, port, name);
		PatchCableManager::registerInput(base);
	}

	std::shared_ptr<InputSocket> base;
};
