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
	//using std::vector;
	
	for(auto i = inputSockets.begin(), 
		end = inputSockets.end(); 
		i != end; ++i){
		
		if ((*i)->isReady() && (*i)->jackDetectorChanged())
			(*i)->jackConnected();
		if (!(*i)->isReady() && (*i)->jackDetectorChanged())
			(*i)->jackDisconnected();
	}

	for(auto o = outputSockets.begin(),
		end = outputSockets.end();
		o != end; ++o){
		if ((*o)->isReady() && (*o)->jackDetectorChanged())
			(*o)->jackConnected();
		if (!(*o)->isReady() && (*o)->jackDetectorChanged())
			(*o)->jackDisconnected();
	}
	return;
}
