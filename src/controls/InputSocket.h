#ifndef __INPUT_SOCKET_H___
#define __INPUT_SOCKET_H___

#include "Socket.h"

//no #define Input(a, id, did, as, p, n) Module::inputSockets.push_back(std::make_shared<InputSocket>(a, id, did, as, p, n))



class InputSocket : public Socket {
protected:
	ControlAddress* address;
	static void removeFromAvailable(std::shared_ptr<InputSocket>&);
	static void removeFromBusy(std::shared_ptr<InputSocket>&);
		
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

	static std::list<std::shared_ptr<InputSocket>> busyInputs;
	static std::list<std::shared_ptr<InputSocket>> availableInputs;

	static void connect(std::shared_ptr<InputSocket>&);
	static void setAvailable(std::shared_ptr<InputSocket>&);
	static void setAvailable(unsigned int);
	static void setBusy(std::shared_ptr<InputSocket>&);
	static inline void disconnect(std::shared_ptr<InputSocket>& i ) { removeFromAvailable(i); removeFromBusy(i); };
};


#endif