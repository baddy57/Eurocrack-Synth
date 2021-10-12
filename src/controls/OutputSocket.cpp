#include "OutputSocket.h"

#include "InputSocket.h"
#include "../PatchCable.h"

#include <list>
#include <cstdint>
#include <Audio.h>
extern ILI9341_t3 tft;

std::list<OutputSocket*> OutputSocket :: withJack = {};

//ctor mono
OutputSocket :: OutputSocket
(
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n="mono out"	
)
:
	address(new OutputSocketAddress(slotAddress, id)), 
	jackDetector(slotAddress, detectorId), 
	index(i),
	name(n),
	voices(1),
	linkedStream0(as)
{
	attachedInputs={};
}





//ctor poly
OutputSocket :: OutputSocket
(
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	//std::reference_wrapper<AudioStream>* ass,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n="poly out"	
)
:
	address(new OutputSocketAddress(slotAddress, id)), 
	jackDetector(slotAddress, detectorId), 
	index(i),
	name(n),
	voices(POLYPHONY),
	linkedStream0(as0),
	linkedStream1(as1),
	linkedStream2(as2),
	linkedStream3(as3)
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
	withJack.push_back(this);
	PatchCable :: updateCables();
	return;
}


//removes OS from withjack and frees any attached IS
//PatchCable :: updateCables() will handle everything else (deleting connections etc)
void
OutputSocket :: jackDisconnected () {
	tft.print("        --OUTPUT: ");
	tft.println(name);
	withJack.remove(this);//works?
	
	
	for (auto i=attachedInputs.begin(),
				end=attachedInputs.end();
				i!=end; ++i)
	{
		(*i)->removeAttachedCable();//202009050026
		InputSocket::withJack.push_back(*i);
		// tft.println("removed connection to input");
	}
	attachedInputs.clear();
	return;
}

void
OutputSocket :: addAttachedInput (InputSocket* i) {
	attachedInputs.push_back(i);
	return;
}

void
OutputSocket :: removeAttachedInput (InputSocket* i) {
	attachedInputs.remove(i); //could cause problems
	return;
}