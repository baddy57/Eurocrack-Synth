#include "Module.h"
#include "HardwareCfg.h"
#include "PatchCable.h"

//definition of static members
std::vector<std::shared_ptr<InputSocket>> Module::inputSockets;

std::vector<std::shared_ptr<OutputSocket>> Module::outputSockets;

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){
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
}