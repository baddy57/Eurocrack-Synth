#include "InputSocket.h"

#include <list>
#include <functional>
#include <cstdint>

#include "../core/PatchCable.h"

/// @brief input sockets that are connected to a patch cable and actively receiving a stream
std::list<std::shared_ptr<InputSocket>> InputSocket::busyInputs;

/// @brief input sockets that are connected to a patch cable and available for connection
std::list<std::shared_ptr<InputSocket>> InputSocket::availableInputs;

// ctor MONO ONLY
InputSocket ::InputSocket( // param
	const Address &slotAddress,
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream &as,
	uint_fast8_t i,
	const char *n)
	// init list
	: Socket(slotAddress, detectorId, as, i, n), address(new ControlAddress(slotAddress, id))
{
	uid = address->_id;
}

// POLY CTOR
InputSocket ::InputSocket( // param
	const Address &slotAddress,
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream &as0,
	AudioStream &as1,
	AudioStream &as2,
	AudioStream &as3,
	uint_fast8_t i,
	const char *n)
	// init list
	: Socket(slotAddress, detectorId, as0, as1, as2, as3, i, n), address(new ControlAddress(slotAddress, id))

{
	uid = address->_id;
}

bool InputSocket::isReceiving() const
{
	address->setForReading();

	return !digitalRead(address->getPin());
}

void InputSocket::removeFromAvailable(std::shared_ptr<InputSocket> i)
{
	for (auto it = availableInputs.begin(), end = availableInputs.end(); it != end; ++it)
		if ((*it)->uid == i->uid)
		{
			availableInputs.erase(it);
			return;
		}
}

void InputSocket::removeFromBusy(std::shared_ptr<InputSocket> i)
{
	for (auto it = busyInputs.begin(), end = busyInputs.end(); it != end; ++it)
		if ((*it)->uid == i->uid)
		{
			busyInputs.erase(it);
			return;
		}
}

void InputSocket::setAvailable(std::shared_ptr<InputSocket> i)
{
	availableInputs.push_back(i);

	if(i->state == SocketState::BUSY)
		removeFromBusy(i);

	i->state = SocketState::AVAILABLE;
}

void InputSocket::setBusy(std::shared_ptr<InputSocket> i)
{
	busyInputs.push_back(i);

	if(i->state == SocketState::AVAILABLE)
		removeFromAvailable(i);

	i->state = SocketState::BUSY;
}

void InputSocket::setInactive(std::shared_ptr<InputSocket> i)
{
	if(i->state == SocketState::AVAILABLE)
		removeFromAvailable(i);

	else if(i->state == SocketState::BUSY)
		removeFromBusy(i);

	i->state = SocketState::INACTIVE;
}