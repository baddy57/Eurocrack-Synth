#include "CONTROLS_InputSocket.h"

#include "Control.h"
#include "HardwareCfg.h"
extern ILI9341_t3 tft;

#include "Address.h"

#include <list>
#include <functional>
#include <cstdint>

#include "PatchCable.h"

std::list<InputSocket*> InputSocket :: withJack = {};


//ctor MONO ONLY
InputSocket :: InputSocket
(	//param
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n="mono in"
//	uint_fast8_t v = 1,
)
:	//init list
	address(new ControlAddress(slotAddress, id)), 
	jackDetector(slotAddress, detectorId),
	index(i), 
	attachedCable(NULL),
	voices(1),
	name(n),
	linkedStream0(as)
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
	
	//std::reference_wrapper<AudioStream>* ass,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n="poly in"
//	uint_fast8_t v = 1,
)
:	//init list
	address(new ControlAddress(slotAddress, id)), 
	jackDetector(slotAddress, detectorId),
	index(i), 
	attachedCable(NULL),
	voices(POLYPHONY),
	name(n),
	linkedStream0(as0),
	linkedStream1(as1),
	linkedStream2(as2),
	linkedStream3(as3)
{	//def
	// for(uint_fast8_t j=0; j<POLYPHONY; ++j)
		// linkedStream[j] = ass[j];
	
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
	withJack.push_back(this);
	PatchCable :: updateCables();
	return;
}

void
InputSocket :: jackDisconnected () {
	tft.print("        --INPUT: ");
	tft.println(name);
	withJack.remove(this);
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