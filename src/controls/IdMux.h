#ifndef __ID_MUX_H__
#define __ID_MUX_H__

#include "../Address.h"
#include <cstdint>
#include <Wire.h>


class IdMux {
	private:
		uint_fast8_t moduleId;
	public:
		IdMux(const Address&);
		inline uint_fast8_t getModuleId()const { return moduleId; }
};

#endif
