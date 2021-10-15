#include "PatchCable.h"

extern ILI9341_t3 tft;

//definition of static member
std::list<PatchCable*> PatchCable :: activeConnections = {};
std::list<std::shared_ptr<OutputSocket>> PatchCable::outputsWithJack;
std::list<std::shared_ptr<InputSocket>> PatchCable::inputsWithJack;

enum connection_type_enum {M2M, M2P, P2M, P2P};


//constructor

PatchCable :: PatchCable (std::shared_ptr<OutputSocket> out, std::shared_ptr<InputSocket> in){
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
PatchCable ::addFromInput(std::shared_ptr<InputSocket> i){
	inputsWithJack.push_front(i);
	searchForCablesToAdd();
	return;
}
void
PatchCable::addFromOutput(std::shared_ptr<OutputSocket> o) {
	outputsWithJack.push_front(o);
	searchForCablesToAdd();
	return;
}
void 
PatchCable::searchForCablesToAdd() {

	for (auto out = outputsWithJack.begin(),
		end = outputsWithJack.end();
		out != end; ++out) {

		for (auto in = inputsWithJack.begin(),
			end2 = inputsWithJack.end();
			in != end2;) {

			if (checkConnection(*out, *in)) {
				activeConnections.push_front(new PatchCable(*out, *in));
				inputsWithJack.erase(in++); ////OK 202009061218//elements removed, which are destroyed.
			}
			else ++in;
		}

	}
}

bool
PatchCable :: checkConnection (std::shared_ptr<OutputSocket> out, std::shared_ptr<InputSocket> in) {
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

void 
PatchCable::deleteFromInput(std::shared_ptr<InputSocket> input) {
	inputsWithJack.remove(input);
	for (auto c = activeConnections.begin(), end = activeConnections.end(); c != end; ++c) 
		if 	((*c)->getInputSocket() == input) {
			activeConnections.remove(*c);
			return; //i have to destroy only one cable
		}
}

void 
PatchCable::deleteFromOutput(std::shared_ptr<OutputSocket>output) {
	outputsWithJack.remove(output);
	for (auto c = activeConnections.begin(), end = activeConnections.end();	c != end; ++c)
		if ((*c)->getOutputSocket() == output)
			activeConnections.remove(*c);
	return;
}