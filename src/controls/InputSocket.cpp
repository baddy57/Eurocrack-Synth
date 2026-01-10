#include "InputSocket.h"

#include <list>
#include <functional>
#include <cstdint>

#include "../core/PatchCable.h"

/// @brief list of input sockets that are connected to a patch cable and actively receiving audio
std::list<InputSocket_p> InputSocket::busyInputs;

/// @brief list of input sockets that are connected to a patch cable and available for connection
std::list<InputSocket_p> InputSocket::availableInputs;

// ctor MONO ONLY
InputSocket ::InputSocket( // param
	const Address &slotAddress,
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream &as,
	uint_fast8_t i,
	const char *n)
	// init list
	: Socket(slotAddress, detectorId, as, i, n), address(new ControlAddress(slotAddress, id)), p2m_mixer(new AudioMixer4())
{
	socket_uid = address->_id;

	p2m_status = false;
	p2m_on();
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
	socket_uid = address->_id;
	
	p2m_status = false;

	p2m_on();
}

bool InputSocket::isReceiving() const
{
	address->setForReading();

	return !digitalRead(address->getPin());
}

void InputSocket::p2m_on()
{
	if (p2m_status)
		return;

	p2m_link = new AudioConnection(*p2m_mixer, 0, linkedStream0, audioStream_port);

	p2m_status = true;
}

void InputSocket::p2m_off()
{
	if (!p2m_status)
		return;

	p2m_link->disconnect();
	
	delete p2m_link;

	p2m_status = false;
}

void InputSocket::removeFromAvailable(InputSocket_p &i)
{
	for (auto it = availableInputs.begin(), end = availableInputs.end(); it != end; ++it)
		if ((*it)->socket_uid == i->socket_uid)
		{
			availableInputs.erase(it);
			return;
		}
}

void InputSocket::removeFromBusy(InputSocket_p &i)
{
	for (auto it = busyInputs.begin(), end = busyInputs.end(); it != end; ++it)
		if ((*it)->socket_uid == i->socket_uid)
		{
			busyInputs.erase(it);
			return;
		}
}

/// @brief sets a as AVAILABLE. currently unused as patchcable refers to sockets via uids
void InputSocket::setAvailable(InputSocket_p &i)
{
	availableInputs.push_back(InputSocket_p(i));

	if((*i)->state == BUSY)
		removeFromBusy(i);

	(*i)->state = AVAILABLE;
}

/// @brief sets an input socket as AVAILABLE by its unique identifier. OBSOLETE
void InputSocket::setAvailable(unsigned int uid)
{
	for (auto inputSocket = busyInputs.begin(), end = busyInputs.end(); inputSocket != end; ++inputSocket)
	{
		if ((*inputSocket)->socket_uid == uid)
		{
			availableInputs.push_back(InputSocket_p(*inputSocket));
			busyInputs.erase(inputSocket);

			(*inputSocket)->state = AVAILABLE;

			return;
		}
	}
}

void InputSocket::setBusy(InputSocket_p &i)
{
	busyInputs.push_back(InputSocket_p(i));

	if((*i)->state == AVAILABLE)
		removeFromAvailable(i);

	(*i)->state = BUSY;
}

void InputSocket::setInactive(InputSocket_p &i)
{
	if((*i)->state == AVAILABLE)
		removeFromAvailable(i);
	
	else if((*i)->state == BUSY)
		removeFromBusy(i);

	(*i)->state = INACTIVE;
}