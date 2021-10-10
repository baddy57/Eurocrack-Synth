#ifndef __ID_MUX_H__
#define __ID_MUX_H__ 1

#include "Address.h"
#include <cstdint>
#include <Wire.h>

class IdMux {
	private:
		IdMuxAddress address;
	public:
		IdMux(const Address&);
		uint_fast8_t read()const;
};

#endif
