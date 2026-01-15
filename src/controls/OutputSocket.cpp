#include "OutputSocket.h"

#include <Audio.h>

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