#pragma once

#include <memory>
#include <cstdint>
#include <Audio.h>

#include "address.h"
#include "../controls/OutputSocket.h"
#include "../services/Connections.h"

struct ModuleOutput {
	inline  ModuleOutput(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono out")
	{
		base = std::make_shared<OutputSocket>(a, id, detectorId, as, port, name);
		PatchCableManager::registerOutput(base);
	}

	inline ModuleOutput(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly out")
	{
		base = std::make_shared<OutputSocket>(a, id, detectorId, as0, as1, as2, as3, port, name);
		PatchCableManager::registerOutput(base);
	}

	std::shared_ptr<OutputSocket> base;
};
