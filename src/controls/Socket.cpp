#include "Socket.h"

//definition of static member
std::list<Socket*> Socket::outputsWithJack = {};
std::list<Socket*> Socket::inputsWithJack = {};


Socket::Socket(
	const Address& slotAddress,
	uint_fast8_t detectorId, 
	AudioStream& as, 
	uint_fast8_t i, 
	const char* n
):
	jackDetector(slotAddress, detectorId),
	name(n),
	voicesCount(1),
	index(i),
	linkedStream0(as),
	linkedStream1(as),//references need to be initialized
	linkedStream2(as),
	linkedStream3(as)

{

}


Socket::Socket(
	const Address& slotAddress,
	uint_fast8_t detectorId,
	AudioStream& as0,
	AudioStream& as1,
	AudioStream& as2,
	AudioStream& as3,
	uint_fast8_t i,
	const char* n
) :
	jackDetector(slotAddress, detectorId),
	name(n),
	voicesCount(POLYPHONY),
	index(i),
	linkedStream0(as0),
	linkedStream1(as1),
	linkedStream2(as2),
	linkedStream3(as3)
{

}
