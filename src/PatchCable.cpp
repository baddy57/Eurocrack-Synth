#include "PatchCable.h"

// #include "controls/InputSocket_POLY.h"
// #include "controls/OutputSocket_POLY.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"

#include <Audio.h>
//#include <list>
//#include <vector>
#include "HardwareCfg.h"
extern ILI9341_t3 tft;

//definition of static member
std::list<PatchCable*> PatchCable :: activeConnections = {}; 

enum connection_type_enum {M2M, M2P, P2M, P2P};


//generic constructor
#ifndef POLYPHONIC
	PatchCable :: PatchCable (OutputSocket* out, InputSocket* in){
		//begin handling underlying connections
		if 		(out->voicesCount==1 && in->voicesCount==1)
		{
			connectionType = M2M;
			//in->p2m_off();
			ac[0] = new AudioConnection (	out->getLinkedStream(),
											out->getIndex(),
											in->p2m_mixer,//getLinkedStream(),
											0);//getIndex()	);
		
		}
		else if	(out->voicesCount==1 && in->voicesCount>1)
		{
			connectionType = M2P;
			for(uint_fast8_t i=0; i<POLYPHONY; ++i)
				ac[i]=new AudioConnection(	out->getLinkedStream(), 
											out->getIndex(), 
											in->getLinkedStream(), 
											i);
		}
		else if	(out->voicesCount>1 && in->voicesCount==1)
		{
			connectionType = P2M;
			in->p2m_on();
			for(uint_fast8_t i=0; i<POLYPHONY; ++i)
				ac[i]=new AudioConnection(	out->getLinkedStream(i), 
											out->getIndex(), 
											in->p2m_mixer, 
											i);
		}
		else /*if(out->voicesCount>1 && in->voicesCount>1)*/
		{
			connectionType = P2P;
			for(uint_fast8_t i=0; i<POLYPHONY; ++i)
				ac[i]=new AudioConnection(	out->getLinkedStream(i), 
											out->getIndex(), 
											in->getLinkedStream(i), 
											in->getIndex());
		}
		//end

		_out = out;
		_in = in;
		in->setAttachedCable(this);//setAttachedOutput //no cross dep
		out->addAttachedInput(in);
		
		tft.print(out->getName());
		tft.print(" >>> ");
		tft.print(in->getName());
		switch (connectionType)
		{
			case M2M: tft.println(" M2M"); break;
			case M2P: tft.println(" M2P"); break;
			case P2M: tft.println(" P2M"); break;
			case P2P: tft.println(" P2P");
		}
	}
#endif

#ifdef POLYPHONIC 
	//ctor MONO TO MONO
	PatchCable :: PatchCable (OutputSocket* out, InputSocket* in){
		// tft.println("PatchCable :: PatchCable(m2m)");
		connectionType = M2M;
		//in->p2m_off();
		ac[0] = new AudioConnection (	out->getLinkedStream(),
										out->getIndex(),
										in->p2m_mixer,//getLinkedStream(),
										0);//getIndex()	);
		linkSockets(out,in);
	}
	//ctor MONO TO POLY
	PatchCable :: PatchCable(OutputSocket* out, InputSocket_POLY* in){
		// tft.println("PatchCable :: PatchCable(m2p)");
		connectionType = M2P;
		for(uint_fast8_t i=0; i<POLYPHONY; ++i)
			ac[i]=new AudioConnection(	out->getLinkedStream(), 
										out->getIndex(), 
										in->getLinkedStream(), 
										i);
		linkSockets(out,in);
	}
	//ctor POLY TO MONO
	PatchCable :: PatchCable(OutputSocket_POLY* out, InputSocket* in){
		// tft.println("PatchCable :: PatchCable(p2m)");
		connectionType = P2M;
		in->p2m_on();
		for(uint_fast8_t i=0; i<POLYPHONY; ++i)
			ac[i]=new AudioConnection(	out->getLinkedStream(i), 
										out->getIndex(), 
										in->p2m_mixer, 
										i);
		linkSockets(out,in);
	}
	//ctor POLY TO POLY
	PatchCable :: PatchCable(OutputSocket_POLY* out, InputSocket_POLY*in){
		// tft.println("PatchCable :: PatchCable(p2p)");
		connectionType = P2P;
		for(uint_fast8_t i=0; i<POLYPHONY; ++i)
			ac[i]=new AudioConnection(	out->getLinkedStream(i), 
										out->getIndex(), 
										in->getLinkedStream(i), 
										in->getIndex());
		linkSockets(out,in);
	}

	void
	PatchCable :: linkSockets(OutputSocket* out, InputSocket* in){
		// tft.println("PatchCable :: linkSockets");
		_out = out;
		_in = in;
		in->setAttachedCable(this);
		out->addAttachedInput(in);
		
		tft.print(out->getName());
		tft.print(" >>> ");
		tft.print(in->getName());
		switch (connectionType)
		{
			case M2M: tft.println(" M2M"); break;
			case M2P: tft.println(" M2P"); break;
			case P2M: tft.println(" P2M"); break;
			case P2P: tft.println(" P2P");
		}
	}
#endif

//dtor
PatchCable :: ~PatchCable(){
	switch (connectionType)
	{
		case M2M:{
		//	ac[0]->disconnect();
			delete ac[0];
			break;
		}
		case M2P:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
			//	ac[i]->disconnect();
				delete ac[i];
			}
			break;
		}
		case P2M:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
			//	ac[i]->disconnect();
				delete ac[i];
			}
			_in->p2m_off();
			break;
		}
		case P2P:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
			//	ac[i]->disconnect();
				delete ac[i];
			}
			break;
		}
	}
	tft.print(_out->getName());
	tft.print(" XXX ");
	tft.println(_in->getName());
}


void
PatchCable :: updateCables(){
	for(auto out = OutputSocket::outputsWithJack.begin(),
		end = OutputSocket::outputsWithJack.end();
		out != end; ++out){
			
		for(auto in = InputSocket::inputsWithJack.begin(),
			end2 = InputSocket::inputsWithJack.end();
			in != end2;){
			
			if (checkConnection (*out, *in)){
				activeConnections.push_front(new PatchCable (*out, *in));
				InputSocket::inputsWithJack.erase(in++); ////OK 202009061218//elements removed, which are destroyed.
			 }
			 else ++in;
		}
		
	}
//	tft.println("updating cables to remove");
	// for (auto c = activeConnections.begin(),
			// end = activeConnections.end();
			// c != end; /* ++c */){
		// if(!checkConnection ((*c)->getOutputSocket(), (*c)->getInputSocket())){
			// tft.println("removing cable");
			// begin = activeConnections.erase(c);
		/* 	activeConnections.remove(*c); */
		// }
		// else ++c;
	// }
	return;
}

bool
PatchCable :: checkConnection (const OutputSocket* out, const InputSocket* in) {
	out->sendSignal();
	if (in->isReceiving()){
		out->resetSignal();
		return true;
	}
	else{
		out->resetSignal();
		return false;
	}
}