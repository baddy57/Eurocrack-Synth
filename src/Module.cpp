#include "Module.h"

#include <Audio.h>
#include <cstdint>

#include "HardwareCfg.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
//#include "Control.h"
//#include "PatchCable.h"

//definition of static members
std::vector<InputSocket*> Module::inputSockets;
std::vector<OutputSocket*> Module::outputSockets;

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){}

void
Module :: updateConnections(){
	
	for(auto i = inputSockets.begin(), 
		end = inputSockets.end(); 
		i != end; ++i){
		
		if ((*i)->isReady() && (*i)->jackDetectorChanged())//jack has just been plugged
			(*i)->jackConnected();
		else
		if (!(*i)->isReady() && (*i)->jackDetectorChanged())//jack has just been unplugged
			(*i)->jackDisconnected();
	}

	for(auto o = outputSockets.begin(),
		end = outputSockets.end();
		o != end; ++o){
		if ((*o)->isReady() && (*o)->jackDetectorChanged())//jack has just been plugged
			(*o)->jackConnected();
		else
		if (!(*o)->isReady() && (*o)->jackDetectorChanged())//jack has just been unplugged
			(*o)->jackDisconnected();
	}
	return;
}
