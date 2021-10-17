#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <cstdint>
#include "../Address.h"
#include "Switch.h"
#include <Audio.h>
#include <list>
#include <memory>
#include <cassert>

class Socket {
protected:
	Switch jackDetector;
	const char* name;

	AudioStream& linkedStream0;
	AudioStream& linkedStream1;
	AudioStream& linkedStream2;
	AudioStream& linkedStream3;
	uint_fast8_t audioStream_port;
public:
	//mono
	Socket(
		const Address&,
		uint_fast8_t,
		AudioStream&,
		uint_fast8_t,
		const char*);
	//poly
	Socket(
		const Address&,
		uint_fast8_t,
		AudioStream&,
		AudioStream&,
		AudioStream&,
		AudioStream&,
		uint_fast8_t,
		const char*);

	uint_fast8_t voicesCount;
	inline bool isReady() { return !jackDetector.b_read(); };
	inline bool jackDetectorChanged() { return jackDetector.wasUpdated(); };
	inline bool jackJustPlugged() { return (isReady() && jackDetectorChanged()); }
	inline bool jackJustUnplugged() { return (!isReady() && jackDetectorChanged()); }
	inline AudioStream& getLinkedStream(uint_fast8_t i = 0) {
		assert(i < 4);
		switch (i) {
		case 0: return linkedStream0;
		case 1: return linkedStream1;
		case 2: return linkedStream2;
		case 3: return linkedStream3;
		};
		return linkedStream0;
	}

	inline uint_fast8_t getIndex()const { return audioStream_port; };
	inline const char* getName()const { return name; };

};

#endif // !__SOCKET_H__
