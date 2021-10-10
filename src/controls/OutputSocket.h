#ifndef __OUTPUT_SOCKET_H___
#define __OUTPUT_SOCKET_H___ 1

#include "Address.h"
#include "CONTROLS_InputSocket.h"
#include "CONTROLS_Switch.h"
#include "HardwareCfg.h"

#include <cstdint>
#include <Audio.h>
#include <list>

class InputSocket;
class PatchCable;

extern ILI9341_t3 tft;

class OutputSocket {
	protected:
		std::list<InputSocket*> attachedInputs;
		
		char* name;
		OutputSocketAddress* address;//
		Switch jackDetector;
		AudioStream& linkedStream0;
		AudioStream& linkedStream1;
		AudioStream& linkedStream2;
		AudioStream& linkedStream3;
		//AudioStream* linkedStream[POLYPHONY];
		uint_fast8_t index;
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
						//std::reference_wrapper<AudioStream>*,
						AudioStream&,
						AudioStream&,
						AudioStream&,
						AudioStream&,
						uint_fast8_t,
						const char* = "poly out");
		uint_fast8_t toInt(){return address->toInt();}; ///////////////2021.01.14
		uint_fast8_t voices;
		void sendSignal() const;
		void resetSignal() const;
		static std::list<OutputSocket*> withJack;
		void jackConnected();
		void jackDisconnected();
		void addAttachedInput(InputSocket*);
		void removeAttachedInput(InputSocket*);
		bool isReady()									{return !jackDetector.b_read();};
		bool jackDetectorChanged()						{return jackDetector.wasUpdated();};
		
		AudioStream& getLinkedStream(uint_fast8_t i=0)  {switch(i){ 
															case 0: return linkedStream0;
															case 1: return linkedStream1;
															case 2: return linkedStream2;
															case 3: return linkedStream3;};}
		uint_fast8_t getIndex()const					{return index;};
		const char*  getName()const						{return name;};
};



#endif