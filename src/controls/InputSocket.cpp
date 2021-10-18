#include "InputSocket.h"

#include <list>
#include <functional>
#include <cstdint>

#include "../PatchCable.h"


std::list<std::shared_ptr<InputSocket>> InputSocket::busyInputs;
std::list<std::shared_ptr<InputSocket>> InputSocket::availableInputs;

//ctor MONO ONLY
InputSocket :: InputSocket
(	//param
	const Address& slotAddress, 
	uint_fast8_t id,
	uint_fast8_t detectorId,
	AudioStream& as,
	uint_fast8_t i,
	const char* n
)
		//init list
	:	Socket(slotAddress, detectorId, as, i, n)
	,	address(new ControlAddress(slotAddress, id))
	,	p2m_mixer(new AudioMixer4())
{
	socket_uid = address->_id;

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
	const char* n
)
	//init list
	:	Socket(slotAddress, detectorId, as0, as1, as2, as3, i, n)
	,	address(new ControlAddress(slotAddress, id))
	
{
	socket_uid = address->_id;
	p2m_status=false;
	p2m_on();
}



bool
InputSocket :: isReceiving() const {
	address->setForReading();
	return !digitalRead(address->getPin());
}

void 
InputSocket :: p2m_on(){
	if(p2m_status) return;
	p2m_link = new AudioConnection(*p2m_mixer,0,linkedStream0, audioStream_port);
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

typedef std::shared_ptr<InputSocket> is_ptr;

//ok
void InputSocket::removeFromAvailable(is_ptr i) {
	for (auto it = availableInputs.begin(), end = availableInputs.end(); it != end; ++it)
		if ((*it)->socket_uid == i->socket_uid)
			availableInputs.erase(it);

}

//ok
void InputSocket::removeFromBusy(is_ptr i) {
	for (auto it = busyInputs.begin(), end = busyInputs.end(); it != end; ++it)
		if ((*it)->socket_uid == i->socket_uid)
			busyInputs.erase(it);

}

//ok
void InputSocket::connect(is_ptr i) {
	availableInputs.push_back(is_ptr(i));

}

void InputSocket::setAvailable(is_ptr i) {
	availableInputs.push_back(is_ptr(i));
	removeFromBusy(i);

	return;
}

//ok
void InputSocket::setBusy(is_ptr i) {
	busyInputs.push_back(is_ptr(i));
	removeFromAvailable(i);

	return;
}
