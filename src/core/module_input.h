#pragma once

#include <memory>
#include <cstdint>
#include <Audio.h>

#include "address.h"
#include "../controls/InputSocket.h"
#include "../services/Connections.h"

struct ModuleInput {
	inline  ModuleInput(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono in")
	{
		base = std::make_shared<InputSocket>(a, id, detectorId, as, port, name);
		PatchCableManager::registerInput(base);
	}

	inline ModuleInput(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly in")
	{
		base = std::make_shared<InputSocket>(a, id, detectorId, as0, as1, as2, as3, port, name);
		PatchCableManager::registerInput(base);
	}

	std::shared_ptr<InputSocket> base;
};
