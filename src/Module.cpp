#include "Module.h"

#include <Audio.h>
//#include <vector>
#include <cstdint>

#include "HardwareCfg.h"
extern ILI9341_t3 tft;
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "Control.h"
#include "PatchCable.h"

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){
	
}

void
Module :: updateConnections(){
	//using std::vector;
	
	for(auto i = inputSocket.begin(), 
		end = inputSocket.end(); 
		i != end; ++i){
		
		if ((*i)->isReady() && (*i)->jackDetectorChanged())
			(*i)->jackConnected();
		if (!(*i)->isReady() && (*i)->jackDetectorChanged())
			(*i)->jackDisconnected();
	}

	for(auto o = outputSocket.begin(),
		end = outputSocket.end();
		o != end; ++o){
		if ((*o)->isReady() && (*o)->jackDetectorChanged())
			(*o)->jackConnected();
		if (!(*o)->isReady() && (*o)->jackDetectorChanged())
			(*o)->jackDisconnected();
	}
	return;
}
