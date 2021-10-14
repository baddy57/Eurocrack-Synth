#ifndef __OUTPUT_SOCKET_H___
#define __OUTPUT_SOCKET_H___
#include "Socket.h"
#include "../HardwareCfg.h"



class PatchCable;

class OutputSocket : public Socket {
protected:
	OutputSocketAddress* address;
	std::list<Socket*> attachedInputs;
	//AudioStream* linkedStream[POLYPHONY];
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
	void addAttachedInput(Socket*);
	void removeAttachedInput(Socket*);
	void removeAttachedCable()override {  }
	virtual void jackConnected()override;
	virtual void jackDisconnected()override;
	inline uint_fast8_t toInt() { return address->toInt(); }; ///////////////2021.01.14

};



#endif