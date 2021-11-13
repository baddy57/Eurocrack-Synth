#include "OutputSocket.h"

#include "InputSocket.h"
#include "../PatchCable.h"

#include <list>
#include <Audio.h>

extern ILI9341_t3 tft;

typedef std::shared_ptr<OutputSocket> os_ptr;

std::list<os_ptr> OutputSocket::availableOutputs;

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

OutputSocket::~OutputSocket() {

}

void 
OutputSocket :: sendSignal() const {
	resetSignal();
	address->setForWriting();
	digitalWrite(address->getPin(), LOW);
	delayMicroseconds(3);//check if necessary
	return;
}

void OutputSocket :: resetSignal() const {
	//address.setForWriting(); is already set
	digitalWrite(WRITE_PIN/*address->getPin()*/, HIGH); 
	return;
}

//ok
void OutputSocket::setAvailable(os_ptr& o){
	availableOutputs.push_back(os_ptr(o));

}

//ok
void OutputSocket::disconnect(os_ptr& out) {

	for (auto o = availableOutputs.begin(), end = availableOutputs.end(); o != end; ++o) {
		if ((*o)->socket_uid == out->socket_uid) {
			availableOutputs.erase(o);
			return;
		}
	}

}