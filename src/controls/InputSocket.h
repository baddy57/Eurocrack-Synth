#pragma once

#include "Socket.h"

/// @brief an input socket that can receive audio signals

// input sockets can have three states:
// INACTIVE: not connected to any patch cable
// AVAILABLE: connected to a patch cable, but not receiving audio
// BUSY: connected to a patch cable and receiving audio

// for faster searching, two lists are kept:
// - availableInputs: input sockets that are AVAILABLE
// - busyInputs: input sockets that are BUSY

class InputSocket : public Socket {
protected:
	ControlAddress* address;

	static void removeFromAvailable(std::shared_ptr<InputSocket>);
	static void removeFromBusy(std::shared_ptr<InputSocket>);
		
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

	static std::list<std::shared_ptr<InputSocket>> busyInputs;
	static std::list<std::shared_ptr<InputSocket>> availableInputs;

	static void setAvailable(std::shared_ptr<InputSocket>);
	static void setBusy(std::shared_ptr<InputSocket>);
	static void setInactive(std::shared_ptr<InputSocket>);
};
