#pragma once

#include "Socket.h"

typedef  std::shared_ptr<OutputSocket> OutputSocket_p;

class OutputSocket : public Socket {
protected:
	OutputSocketAddress* address;
	static void removeFromAvailable(OutputSocket_p out);

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
		
	static std::list<OutputSocket_p> availableOutputs;

	static void setAvailable(OutputSocket_p);
	static void setInactive(OutputSocket_p);
};