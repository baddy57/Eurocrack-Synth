#pragma once

#include "socket.h"

/// @brief an input socket that can receive audio signals

class InputSocket : public Socket {

	friend class TestDisplay;

protected:
	ControlAddress* address;

public:
	//mono ctor
	InputSocket(	const Address&,
					uint_fast8_t,
					uint_fast8_t,
					AudioStream&,
					uint_fast8_t,
					const char* = "mono in"	);

	//poly ctor
	InputSocket(	const Address&,
					uint_fast8_t,
					uint_fast8_t,
					AudioStream&,
					AudioStream&,
					AudioStream&,
					AudioStream&,
					uint_fast8_t,
					const char* = "poly in"	);

	bool isReceiving() const;

	unsigned int getId() override { return address->getId(); }
};
