#ifndef __OUTPUT_SOCKET_H___
#define __OUTPUT_SOCKET_H___
#include "Socket.h"

class OutputSocket : public Socket {
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
	~OutputSocket();
	void sendSignal() const;
	void resetSignal() const;
	unsigned int socket_uid;
	static std::list< std::shared_ptr<OutputSocket>> availableOutputs;
	void setAvailable();
	void disconnect();
};



#endif