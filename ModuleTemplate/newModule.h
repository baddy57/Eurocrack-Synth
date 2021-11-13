#pragma once
#include <Module.h>

/// @brief 
class ModuleTemplate : public Module {
protected:
	Potentiometer
		pot0,
		pot1,
		pot2;
	Switch
		sw0,
		sw1;
	Button
		btn0,
		btn1,
		btn2;
	
	AudioStream something;

};

