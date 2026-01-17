#pragma once

#include <memory>
#include <cstdint>
#include <Audio.h>

#include "address.h"
#include "../controls/output_socket.h"
#include "../services/patch_cable_manager.h"

struct ModuleOutput {
	inline  ModuleOutput(const Address& a,
		uint_fast8_t modulePin,
		uint_fast8_t detectormodulePin,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono out")
	{
		base = std::make_shared<OutputSocket>(a, modulePin, detectormodulePin, as, port, name);
		PatchCableManager::registerOutput(base);
	}

	inline ModuleOutput(const Address& a,
		uint_fast8_t modulePin,
		uint_fast8_t detectormodulePin,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly out")
	{
		base = std::make_shared<OutputSocket>(a, modulePin, detectormodulePin, as0, as1, as2, as3, port, name);
		PatchCableManager::registerOutput(base);
	}

	std::shared_ptr<OutputSocket> base;
};
