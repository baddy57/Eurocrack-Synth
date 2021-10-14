#ifndef __INPUT_SOCKET_H___
#define __INPUT_SOCKET_H___
#include "Socket.h"

#include <functional>

// class OutputSocket;
// class InputSocket_POLY;
class PatchCable; //////////// error patchcable destructor not being called because of this

class InputSocket : public Socket {
protected:
	ControlAddress* address;
	PatchCable* attachedCable;
	//AudioStream* linkedStream[POLYPHONY]; //NO
	//AsRefCont* linkedStream[POLYPHONY];
	//std::reference_wrapper<AudioStream> linkedStream[POLYPHONY];
	//...
		
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
					//std::reference_wrapper<AudioStream>*,
					AudioStream&,
					AudioStream&,
					AudioStream&,
					AudioStream&,
					uint_fast8_t,
					const char* = "poly in"	);
	bool isReceiving() const;
	void setAttachedCable(PatchCable* c)	{attachedCable = c;};
	void removeAttachedCable()override{delete attachedCable;};
	void p2m_on();
	void p2m_off();
	bool p2m_status;
	// uint_fast8_t as_input_port;
	AudioConnection* p2m_link;
	AudioMixer4 p2m_mixer;

	virtual void jackConnected()override;
	virtual void jackDisconnected()override;
	inline uint_fast8_t toInt() { return address->toInt(); }; ///////////////2021.01.14
};


#endif