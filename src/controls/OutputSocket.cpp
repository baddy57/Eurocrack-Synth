#include "OutputSocket.h"

#include "InputSocket.h"
#include "../core/PatchCable.h"

#include <list>
#include <Audio.h>

extern ILI9341_t3 tft;

/// @brief list of output sockets that are connected to a patch cable and available for connection
std::list<OutputSocket_p> OutputSocket::availableOutputs;

//ctor mono
OutputSocket :: OutputSocket (
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n	
)
	:	Socket(slotAddress, detectorId, as, i, n)
	,	address(new OutputSocketAddress(slotAddress, id))
	,   socket_uid(address->_id)
{
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
	const char* n	
)
	:	Socket(slotAddress, detectorId, as0, as1, as2, as3, i, n)
	,	address(new OutputSocketAddress(slotAddress, id))
	,   socket_uid(address->_id)
{
}

OutputSocket::~OutputSocket() 
{
}

void OutputSocket :: sendSignal() const 
{
	resetSignal();
	address->setForWriting();
	digitalWrite(address->getPin(), LOW);
	delayMicroseconds(3);//check if necessary
	return;
}

void OutputSocket :: resetSignal() const 
{
	//address.setForWriting(); is already set
	digitalWrite(WRITE_PIN/*address->getPin()*/, HIGH); 
	return;
}

void OutputSocket::removeFromAvailable(OutputSocket_p &out)
{
	for (auto o = availableOutputs.begin(), end = availableOutputs.end(); o != end; ++o) {
		if ((*o)->socket_uid == out->socket_uid) {
			availableOutputs.erase(o);
			return;
		}
	}
}

void OutputSocket::setAvailable(OutputSocket_p& o)
{
	availableOutputs.push_back(OutputSocket_p(o));

	*o->state = AVAILABLE;
}

void OutputSocket::setInactive(OutputSocket_p& out) 
{
	if((*out)->state == AVAILABLE)
		removeFromAvailable(out);
	
	*out->state = INACTIVE;
}