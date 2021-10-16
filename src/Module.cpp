#include "Module.h"

#include <cstdint>

#include "HardwareCfg.h"
#include "PatchCable.h"

//definition of static members
std::vector<std::shared_ptr<InputSocket>> Module::inputSockets;
std::vector<std::shared_ptr<OutputSocket>> Module::outputSockets;

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){}

void
Module :: updateConnections(){
	//for each input
	for(auto i = inputSockets.begin(), 
		end = inputSockets.end(); 
		i != end; ++i)
	{
		//if jack has just been plugged
		if ((*i)->isReady() && (*i)->jackDetectorChanged())
			PatchCable::addFromInput(*i);

		//if jack has just been unplugged
		if (!(*i)->isReady() && (*i)->jackDetectorChanged())			
			PatchCable::deleteFromInput(*i);
	}

	//for each output
	for(auto o = outputSockets.begin(),
		end = outputSockets.end();
		o != end; ++o)
	{
		//if jack has just been plugged
		if ((*o)->isReady() && (*o)->jackDetectorChanged())
			PatchCable::addFromOutput(*o);

		//if jack has just been unplugged
		if (!(*o)->isReady() && (*o)->jackDetectorChanged())
			PatchCable::deleteFromOutput(*o);
	}
	return;
}