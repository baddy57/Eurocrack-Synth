#pragma once

#include "../core/Address.h"

/// @brief a 4051 multiplexer (located on each module board) with some pins bound to GND, to be read as an 8-bit identifier that represents the module type id
class ModuleTypeIdMux {
	private:
		uint_fast8_t moduleId;
	public:
		/// @brief ctor - reads the module identifier from the multiplexer
		ModuleTypeIdMux(const Address&);
		
		/// @brief returns the module identifier previously read
		inline uint_fast8_t getModuleId()const { return moduleId; }
};
