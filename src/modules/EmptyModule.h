#ifndef __EMPTY_MODULE_H__
#define __EMPTY_MODULE_H__

#include "../Module.h"
#include "../Address.h"

class EmptyModule : public Module {
	private:
	
	public:
		//EmptyModule(const Address& a) : Module(a) {};
		void updateValues()override{return;};
};

#endif