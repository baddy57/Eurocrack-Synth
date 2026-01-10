#pragma once

#include <list>
#include <memory>
#include <cassert>

#include <Audio.h>

#include "SocketState.h"
#include "../core/Address.h"
#include "Switch.h"

class Socket
{
protected:
	Switch jackDetector;
	const char *name;

	AudioStream &linkedStream0;
	AudioStream &linkedStream1;
	AudioStream &linkedStream2;
	AudioStream &linkedStream3;

	uint_fast8_t audioStream_port;

	SocketState state = SocketState::INACTIVE;

public:
	// mono
	Socket(
		const Address &,
		uint_fast8_t,
		AudioStream &,
		uint_fast8_t,
		const char *);

	// poly
	Socket(
		const Address &,
		uint_fast8_t,
		AudioStream &,
		AudioStream &,
		AudioStream &,
		AudioStream &,
		uint_fast8_t,
		const char *);

	uint_fast8_t voicesCount;

	unsigned int uid;

	// aka isAvailable
	inline bool hasJack() { return !jackDetector.b_read(); };

	inline bool jackDetectorChanged() { return jackDetector.wasUpdated(); };

	inline bool jackJustPlugged() { return (hasJack() && jackDetectorChanged()); }

	inline bool jackJustUnplugged() { return (!hasJack() && jackDetectorChanged()); }

	inline AudioStream& getLinkedStream(uint_fast8_t i = 0)
	{
		assert(i < 4);

		switch (i)
		{
		case 0: return linkedStream0;
		case 1: return linkedStream1;
		case 2: return linkedStream2;
		case 3: return linkedStream3;
		};

		return linkedStream0;
	}

	inline uint_fast8_t getIndex() const { return audioStream_port; };

	inline const char* getName() const { return name; };
};
