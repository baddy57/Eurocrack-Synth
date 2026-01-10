#include "OutputSocket.h"

#include "InputSocket.h"
#include "../core/PatchCable.h"

#include <list>
#include <Audio.h>

extern ILI9341_t3 tft;

/// @brief output sockets that are connected to a patch cable and available for connection
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
{
	uid = address->_id;
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
{
	uid = address->_id;
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
	digitalWrite(WRITE_PIN, HIGH); 
	return;
}

void OutputSocket::removeFromAvailable(OutputSocket_p out)
{
	for (auto o = availableOutputs.begin(), end = availableOutputs.end(); o != end; ++o) {
		if ((*o)->uid == out->uid) {
			availableOutputs.erase(o);
			return;
		}
	}
}

void OutputSocket::setAvailable(OutputSocket_p out)
{
	availableOutputs.push_back(out);

	*out->state = SocketState::AVAILABLE;
}

void OutputSocket::setInactive(OutputSocket_p out) 
{
	if(out->state == SocketState::AVAILABLE)
		removeFromAvailable(out);
	
	out->state = SocketState::INACTIVE;
}