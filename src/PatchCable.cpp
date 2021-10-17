#include "PatchCable.h"

extern ILI9341_t3 tft;

//definition of static member
std::list<std::unique_ptr<PatchCable>> PatchCable::activeConnections;

enum connection_type_enum {M2M, M2P, P2M, P2P};
//constructor ok
PatchCable :: PatchCable (os_ptr out, is_ptr in){
	//begin handling underlying connections
	if 		(out->voicesCount==1 && in->voicesCount==1)
	{
		connectionType = M2M;
		in->p2m_off();
		ac[0] = new AudioConnection (	out->getLinkedStream(),
										out->getIndex(),
										in->getLinkedStream(), //getLinkedStream(),
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
										*(in->p2m_mixer), 
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

	inputSocket_uid = in->socket_uid;
	outputSocket_uid = out->socket_uid;

	//in cannot accept any other connections until this one is deleted
	in->setBusy();

	/*
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
	*/
}

//dtor
PatchCable :: ~PatchCable(){
	switch (connectionType)
	{
		case M2M:{
			ac[0]->disconnect();
			delete ac[0];
			break;
		}
		case M2P:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
				ac[i]->disconnect();
				delete ac[i];
			}
			break;
		}
		case P2M:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
				ac[i]->disconnect();
				delete ac[i];
			}
			////////////////////////////////////_in->p2m_off();
			break;
		}
		case P2P:{
			for(uint_fast8_t i=0; i<POLYPHONY; ++i){
				ac[i]->disconnect();
				delete ac[i];
			}
			break;
		}
	}
	/*
	tft.print(_out->getName());
	tft.print(" XXX ");
	tft.println(_in->getName());
	*/
}

//ok
void PatchCable ::addFromInput(is_ptr i){
	i->connect();

	searchForCablesToAdd();
}

//ok
void PatchCable::addFromOutput(os_ptr o) {
	o->setAvailable();
	searchForCablesToAdd();
}

//ok
void PatchCable::searchForCablesToAdd() {
	//if there are at least 1 in AND 1 out available
	if(!OutputSocket::availableOutputs.empty() && !InputSocket::availableInputs.empty())
	
	//check connection for every possible couple (out,in)	
	for (auto out = OutputSocket::availableOutputs.begin(), end = OutputSocket::availableOutputs.end(); out != end; ++out) {
		for (auto in = InputSocket::availableInputs.begin(), end2 = InputSocket::availableInputs.end(); in != end2;++in) {
			if (checkConnection(*out, *in)) {
				//if (out,in) are connected, instantiate a patchcable
				activeConnections.push_back(std::make_unique<PatchCable>(*out, *in));
			}
		}
	}
}

//ok
bool PatchCable::checkConnection (os_ptr out, is_ptr in) {
	out->sendSignal();

	if (in->isReceiving()){

		out->resetSignal();
		return true;
	}

	//else
	out->resetSignal();
	return false;
	
}

//ok
void PatchCable::deleteFromInput(is_ptr input) {
	//destroy the patchcable that was disconnected from the socket
	if(!activeConnections.empty())
	for (auto c = activeConnections.begin(), end = activeConnections.end(); c != end; ++c) 
		if 	((*c)->inputSocket_uid == input->socket_uid) {
			activeConnections.erase(c);
			break; //i have to destroy only one cable
		}
	//if input was part of a patchcable, remove input from busy
	//if input was not part of a patchcable, remove input from available
	//input->disconnect handles both cases
	input->disconnect();
	return;
}

void 
PatchCable::deleteFromOutput(os_ptr output) {

	if (!activeConnections.empty()) {
		for (auto c = activeConnections.begin(), end = activeConnections.end(); c != end; ) {
			if ((*c)->outputSocket_uid == output->socket_uid) {
				activeConnections.erase(c);
				if (activeConnections.empty()) break;
				else --end;
			}
			else ++c;
		}
  	}
	output->disconnect();
	return;
}


