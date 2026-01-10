#include "Module.h"
#include "HardwareCfg.h"
#include "PatchCable.h"

//definition of static members
std::vector<InputSocket_p> Module::inputSockets;

std::vector<OutputSocket_p> Module::outputSockets;

AudioControlSGTL5000 Module::_audioCtrl;

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){
	//_audioCtrl.enable();
}

//ok
void Module :: updateConnections(){
	//for each input
	for(auto inputSocket = inputSockets.begin(), end = inputSockets.end(); inputSocket != end; ++inputSocket)
	{
		if ((*inputSocket)->jackJustPlugged())
			PatchCable::onInputSocketConnected(*inputSocket);

		if ((*inputSocket)->jackJustUnplugged())
			PatchCable::onInputSocketDisconnected(*inputSocket);
	}

	//for each output
	for(auto outputSocket = outputSockets.begin(),	end = outputSockets.end(); outputSocket != end; ++outputSocket)
	{
		if ((*outputSocket)->jackJustPlugged())
			PatchCable::onOutputSocketConnected(*outputSocket);

		if ((*outputSocket)->jackJustUnplugged()) 
			PatchCable::onOutputSocketDisconnected(*outputSocket);
	}
	return;
}