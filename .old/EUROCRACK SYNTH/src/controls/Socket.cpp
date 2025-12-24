#include "Socket.h"

Socket::Socket(
	const Address& slotAddress,
	uint_fast8_t detectorId, 
	AudioStream& as, 
	uint_fast8_t i, 
	const char* n
):
	jackDetector(slotAddress, detectorId),
	name(n),
	linkedStream0(as),
	linkedStream1(as),//references need to be initialized
	linkedStream2(as),
	linkedStream3(as)

{
	voicesCount = 1;
	audioStream_port = i;
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
	linkedStream0(as0),
	linkedStream1(as1),
	linkedStream2(as2),
	linkedStream3(as3)
{
	voicesCount = POLYPHONY;
	audioStream_port = i;
}
