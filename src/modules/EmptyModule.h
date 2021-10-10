#ifndef __EMPTY_MODULE_H__
#define __EMPTY_MODULE_H__ 1

#include "Module.h"
#include "Address.h"

class EmptyModule : public Module {
	private:
	
	public:
		EmptyModule(const Address& a) : Module(a) {};
		void updateValues(){return;};
		void updateConnections(){return;};
};

#endif