#pragma once

#include "../core/module.h"

class EmptyModule : public Module {
	private:
	
	public:
		//EmptyModule(const Address& a) : Module(a) {};
		void updateValues()override{return;};
};
