#pragma once

#include "socket.h"

class OutputSocket : public Socket
{
	friend class TestDisplay;

protected:
	OutputSocketAddress* address;

public:
	//mono
	OutputSocket(	const Address&,
		uint_fast8_t,
		uint_fast8_t,
		AudioStream&,
		uint_fast8_t,
		const char* = "mono out");

	//poly
	OutputSocket(	const Address&,
		uint_fast8_t,
		uint_fast8_t,
		AudioStream&,
		AudioStream&,
		AudioStream&,
		AudioStream&,
		uint_fast8_t,
		const char* = "poly out");

	void sendSignal() const;
	void resetSignal() const;
};