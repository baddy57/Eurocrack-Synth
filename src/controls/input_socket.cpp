#include "input_socket.h"

#include <cstdint>

// ctor MONO ONLY
InputSocket ::InputSocket( // param
	const Address &slotAddress,
	uint_fast8_t modulePin,
	uint_fast8_t detectormodulePin,
	AudioStream &as,
	uint_fast8_t i,
	const char *n)
	// init list
	: Socket(slotAddress, detectormodulePin, as, i, n), address(new ControlAddress(slotAddress, modulePin))
{
}

// POLY CTOR
InputSocket ::InputSocket( // param
	const Address &slotAddress,
	uint_fast8_t modulePin,
	uint_fast8_t detectormodulePin,
	AudioStream &as0,
	AudioStream &as1,
	AudioStream &as2,
	AudioStream &as3,
	uint_fast8_t i,
	const char *n)
	// init list
	: Socket(slotAddress, detectormodulePin, as0, as1, as2, as3, i, n), address(new ControlAddress(slotAddress, modulePin))
{
}

bool InputSocket::isReceiving() const
{
	address->setForReading();

	return !digitalRead(address->getArduinoPin());
}