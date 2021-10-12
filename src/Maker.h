#ifndef __MAKER_H__
#define __MAKER_H__

//#include <vector>
#include <cstdint>
#include "Address.h"
#include "Module.h"

class Maker {
	private:
		static std::vector <Address*> addresses;
		static Module* factory(const Address&);
		static uint_fast8_t getModuleID(const Address&);
	public:
		static void createModules(std::vector<Module*>&);
};

#endif