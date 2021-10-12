#ifndef __INPUT_SOCKET_H___
#define __INPUT_SOCKET_H___ 1

#include "../Address.h"
#include "Switch.h"

//#include "PatchCable.h"

#include <cstdint>
#include <Audio.h>
#include <list>
#include <functional>

// class OutputSocket;
// class InputSocket_POLY;
class PatchCable; //////////// error patchcable destructor not being called because of this

extern ILI9341_t3 tft;

class InputSocket{
	protected:
		PatchCable* attachedCable;
		ControlAddress* address;
		Switch jackDetector;
		//AudioStream* linkedStream[POLYPHONY]; //NO
		//AsRefCont* linkedStream[POLYPHONY];
		AudioStream& linkedStream0;
		AudioStream& linkedStream1;
		AudioStream& linkedStream2;
		AudioStream& linkedStream3;
		//std::reference_wrapper<AudioStream> linkedStream[POLYPHONY];
		//...
		uint_fast8_t index;
		const char* name;
		
		
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
		uint_fast8_t toInt(){return address->toInt();}; ///////////////2021.01.14
		bool isReceiving() const;
		static std::list<InputSocket*> withJack;
		void jackConnected();
		void jackDisconnected();
		void setAttachedCable(PatchCable* c)	{attachedCable = c;};
		void removeAttachedCable()				{delete attachedCable;};
		void p2m_on();
		void p2m_off();
		bool p2m_status;
		// uint_fast8_t as_input_port;
		AudioConnection* p2m_link;
		AudioMixer4 p2m_mixer; 
		uint_fast8_t voices;
		bool isReady()							{return !jackDetector.b_read();};
		bool jackDetectorChanged()				{return jackDetector.wasUpdated();};
		
		AudioStream& getLinkedStream(uint_fast8_t i=0)  {switch(i){ 
															case 0: return linkedStream0;
															case 1: return linkedStream1;
															case 2: return linkedStream2;
															case 3: return linkedStream3;};}
		uint_fast8_t getIndex()const			{return index;};		
		const char*  getName()const				{return name;};
};


#endif