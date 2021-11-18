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
										in->getIndex());
										//0);//getIndex()	);
		
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
	InputSocket::setBusy(in);


	
	Serial.print(out->getName());
	Serial.print(" >>> ");
	Serial.print(in->getName());
	switch (connectionType)
	{
		case M2M: Serial.println(" M2M"); break;
		case M2P: Serial.println(" M2P"); break;
		case P2M: Serial.println(" P2M"); break;
		case P2P: Serial.println(" P2P");
	}
	
}

//dtor
PatchCable :: ~PatchCable(){
	switch (connectionType)
	{
		case M2M:{
			//ac[0]->disconnect();
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
	
	/*Serial.print(_out->getName());
	Serial.print(" XXX ");
	Serial.println(_in->getName());
	*/
}

//ok
void PatchCable ::addFromInput(is_ptr i){
	
	InputSocket::connect(i);
	searchForCablesToAdd();
}

//ok
void PatchCable::addFromOutput(os_ptr o) {

	OutputSocket::setAvailable(o);

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
				//post increment the iterator because *in will be removed from available inputs
				activeConnections.push_back(std::make_unique<PatchCable>(*out, *(in++)));
				break; //provvisorio, permette la creazione di un solo cavo per volta --> dangling
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
	InputSocket::disconnect(input);
	return;
}

void 
PatchCable::deleteFromOutput(os_ptr output) {

	if (!activeConnections.empty()) {
		for (auto c = activeConnections.begin(), end = activeConnections.end(); c != end; ) {
			if ((*c)->outputSocket_uid == output->socket_uid) {

				//the inputs that were connected to this output must be set available again!
				InputSocket::setAvailable((*c)->inputSocket_uid);

				activeConnections.erase(c++);
				//if (activeConnections.empty()) break;
				//else --end;
			}
			else ++c;
		}
  	}

	OutputSocket::disconnect(output);

	return;
}


