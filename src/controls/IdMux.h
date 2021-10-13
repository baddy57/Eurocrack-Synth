#ifndef __ID_MUX_H__
#define __ID_MUX_H__

#include "../Address.h"
#include <cstdint>
#include <Wire.h>

struct ModuleId{
	uint_fast8_t decimal;
	char* binaryString;
	bool binary[8];
	ModuleId(uint_fast8_t);
	ModuleId(bool*);
	ModuleId(char*);
};

class IdMux {
	private:
		IdMuxAddress address;
	public:
		ModuleId moduleId;
		IdMux(const Address&);
		uint_fast8_t read()const;
};

#endif
