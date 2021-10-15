#ifndef __INPUT_SOCKET_H___
#define __INPUT_SOCKET_H___
#include "Socket.h"

class InputSocket : public Socket {
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
	void p2m_on();
	void p2m_off();

	bool p2m_status;
	AudioConnection* p2m_link;
	AudioMixer4 p2m_mixer;
};


#endif