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

typedef std::shared_ptr<InputSocket> InputSocket_p;

class InputSocket : public Socket {
protected:
	ControlAddress* address;
	
	static void removeFromAvailable(InputSocket_p&);
	static void removeFromBusy(InputSocket_p&);
		
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
	void p2m_on();
	void p2m_off();

	bool p2m_status;
	AudioConnection* p2m_link;
	AudioMixer4* p2m_mixer;
	unsigned int socket_uid;

	static std::list<InputSocket_p> busyInputs;
	static std::list<InputSocket_p> availableInputs;

	static void setAvailable(InputSocket_p&);
	static void setAvailable(unsigned int);

	static void setBusy(InputSocket_p&);

	static void setInactive(InputSocket_p& i );
};
