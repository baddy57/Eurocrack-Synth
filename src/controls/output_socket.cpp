#include "output_socket.h"

#include <Audio.h>

//ctor mono
OutputSocket :: OutputSocket (
	const Address& slotAddress,
	uint_fast8_t modulePin,
	uint_fast8_t detectormodulePin,
	AudioStream& as,
	uint_fast8_t i,
	const char* n
)
	:	Socket(slotAddress, detectormodulePin, as, i, n)
	,	address(new OutputSocketAddress(slotAddress, modulePin))
{
}

//ctor poly
OutputSocket :: OutputSocket (
	const Address& slotAddress,
	uint_fast8_t modulePin,
	uint_fast8_t detectormodulePin,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n
)
	:	Socket(slotAddress, detectormodulePin, as0, as1, as2, as3, i, n)
	,	address(new OutputSocketAddress(slotAddress, modulePin))
{
}

void OutputSocket :: sendSignal() const
{
	resetSignal();
	address->setForWriting();
	digitalWrite(address->getArduinoPin(), LOW);
	delayMicroseconds(3);//check if necessary
	return;
}

void OutputSocket :: resetSignal() const
{
	//address.setForWriting(); is already set
	digitalWrite(pins::WRITE, HIGH);
	return;
}