#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <cstdint>
#include "../Address.h"
#include "Switch.h"
#include <Audio.h>
#include <list>
class Socket {
protected:
	Switch jackDetector;
	const char* name;

	AudioStream& linkedStream0;
	AudioStream& linkedStream1;
	AudioStream& linkedStream2;
	AudioStream& linkedStream3;
	uint_fast8_t index;
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
	virtual void jackConnected() =0;
	virtual void jackDisconnected() =0;
	inline bool isReady() { return !jackDetector.b_read(); };
	inline bool jackDetectorChanged() { return jackDetector.wasUpdated(); };
	inline AudioStream& getLinkedStream(uint_fast8_t i = 0) {
		switch (i) {
		case 0: return linkedStream0;
		case 1: return linkedStream1;
		case 2: return linkedStream2;
		case 3: return linkedStream3;
			return linkedStream0;
		};
	}

	inline uint_fast8_t getIndex()const { return index; };
	inline const char* getName()const { return name; };
	static std::list<Socket*> inputsWithJack;
	static std::list<Socket*> outputsWithJack;
	virtual void removeAttachedCable()=0;
};

#endif // !__SOCKET_H__
