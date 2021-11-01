#include "Module.h"

#include <cstdint>

#include "HardwareCfg.h"
#include "PatchCable.h"

//definition of static members
std::vector<std::shared_ptr<InputSocket>> Module::inputSockets;
std::vector<std::shared_ptr<OutputSocket>> Module::outputSockets;

/*unsigned int Module::fix(const unsigned int i)
{
	if (i >= 32 && i % 16 >= 8)
		return i;
	if (i >= 32)
		return i - 24;
	if (i % 16 >= 8)
		return i + 24;
	return i;
}*/

//ctor
Module :: Module(const Address& address) : moduleAddress(address), verbose(false){
	//_audioCtrl.enable();
}

//ok
void Module :: updateConnections(){
	//for each input
	for(auto i = inputSockets.begin(), end = inputSockets.end(); i != end; ++i)
	{
		if ((*i)->jackJustPlugged())
			PatchCable::addFromInput(*i);

		if ((*i)->jackJustUnplugged())
			PatchCable::deleteFromInput(*i);
	}

	//for each output
	for(auto o = outputSockets.begin(),	end = outputSockets.end(); o != end; ++o)
	{
		if ((*o)->jackJustPlugged()) {
			PatchCable::addFromOutput(*o);

		}

		if ((*o)->jackJustUnplugged()) {

			PatchCable::deleteFromOutput(*o);

		}
	}
	return;
}