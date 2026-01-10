#pragma once

#include "Socket.h"

class OutputSocket : public Socket 
{
	protected:
	OutputSocketAddress* address;
	static void removeFromAvailable(std::shared_ptr<OutputSocket> out);
	
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
	
	static std::list<std::shared_ptr<OutputSocket>> availableOutputs;
	
	static void setAvailable(std::shared_ptr<OutputSocket>);
	static void setInactive(std::shared_ptr<OutputSocket>);
};