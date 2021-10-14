#include "OutputSocket.h"

#include "InputSocket.h"
#include "../PatchCable.h"

#include <list>
#include <Audio.h>
extern ILI9341_t3 tft;


//ctor mono
OutputSocket :: OutputSocket (
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n="mono out"	
)
:
	Socket(slotAddress, detectorId, as, i, n),
	address(new OutputSocketAddress(slotAddress, id))
{
	attachedInputs={};
}

//ctor poly
OutputSocket :: OutputSocket (
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n="poly out"	
)
:
	Socket(slotAddress, detectorId, as0, as1, as2, as3, i, n),
	address(new OutputSocketAddress(slotAddress, id))
{
	// for(uint_fast8_t j=0;j<POLYPHONY; ++j)
		// linkedStream[j]=ass[j], 
	attachedInputs={};
}















void 
OutputSocket :: sendSignal() const {
	resetSignal();
	address->setForWriting();
	digitalWrite(address->getPin(), LOW);
	delayMicroseconds(3);//check if necessary
	return;
}

void 
OutputSocket :: resetSignal() const {
	//address.setForWriting();
	digitalWrite(WRITE_PIN/*address->getPin()*/, HIGH); 
	//address->getPin() caused crash 202009052133 
	//because OutputSocket was initialized with the wrong constructor 202009060159
	return;
}

void
OutputSocket :: jackConnected () {
	tft.print("     ++OUTPUT: ");
	tft.println(name);
	outputsWithJack.push_back(this);
	PatchCable :: updateCables();
	return;
}


//removes OS from withjack and frees any attached IS
//PatchCable :: updateCables() will handle everything else (deleting connections etc)
void
OutputSocket :: jackDisconnected () {
	tft.print("        --OUTPUT: ");
	tft.println(name);
	outputsWithJack.remove(this);//works?///////////////////////////////////////////////////////////////
	
	
	for (auto i=attachedInputs.begin(),
				end=attachedInputs.end();
				i!=end; ++i)
	{
		(*i)->removeAttachedCable();//202009050026
		//if there was a connection to an input, 
		//and the cable was removed on the output end, 
		//then the jack is still plugged into the inputsocket on the other end
		//so i have to put it back in the list of available inputs
		//because it was removed when patchcable was built
		inputsWithJack.push_back(*i); 
		// tft.println("removed connection to input");
	}
	attachedInputs.clear();
	return;
}

void
OutputSocket :: addAttachedInput (Socket* i) {
	attachedInputs.push_back(i);
	return;
}

void
OutputSocket :: removeAttachedInput (Socket* i) {
	attachedInputs.remove(i); //could cause problems
	return;
}