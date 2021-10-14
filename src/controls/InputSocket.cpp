#include "InputSocket.h"

extern ILI9341_t3 tft;

#include <list>
#include <functional>
#include <cstdint>

#include "../PatchCable.h"



//ctor MONO ONLY
InputSocket :: InputSocket
(	//param
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n="mono in"
)
:	//init list
	Socket(slotAddress, detectorId, as, i, n),
	address(new ControlAddress(slotAddress, id)),
	attachedCable(NULL)
{	//def
	p2m_status=false;
	p2m_on();
}

//POLY CTOR
InputSocket :: InputSocket
(	//param
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n="poly in"
)
:	//init list
	Socket(slotAddress, detectorId, as0, as1, as2, as3, i, n),
	address(new ControlAddress(slotAddress, id)), 
	attachedCable(NULL)
{	
	p2m_status=false;
	p2m_on();
}



bool
InputSocket :: isReceiving() const {
	address->setForReading();
	return !digitalRead(address->getPin());
}

void
InputSocket :: jackConnected () {
	tft.print("     ++INPUT: ");
	tft.println(name);
	inputsWithJack.push_back(this);
	PatchCable :: updateCables();
	return;
}

void
InputSocket :: jackDisconnected () {
	tft.print("        --INPUT: ");
	tft.println(name);
	inputsWithJack.remove(this); ///CALLS INPUTSOCKET DESTRUCTOR!!!!!!!!
	delete attachedCable; 
	return;
}


void 
InputSocket :: p2m_on(){
	if(p2m_status) return;
	p2m_link = new AudioConnection(p2m_mixer,0,linkedStream0,index); 
	p2m_status=true;
	return;
}

void 
InputSocket :: p2m_off(){
	if(!p2m_status) return;
	p2m_link->disconnect();
	delete p2m_link;
	p2m_status=false;
	return;
}