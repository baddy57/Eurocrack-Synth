#include "PatchCableConnectionType.h"
#include "PatchCable.h"
#include <memory>

extern ILI9341_t3 tft;

// definition of static member
std::list<std::unique_ptr<PatchCable>> PatchCable::activeCables;

//ctor
PatchCable::PatchCable(OutputSocket_p out, InputSocket_p in)
{
	// begin handling underlying connections

	if (!out->isPolyphonic && !in->isPolyphonic)
	{
		connectionType = PatchCableConnectionType::M2M;

		ac[0] = new AudioConnection(out->getLinkedStream(),
									out->getIndex(),
									in->getLinkedStream(),
									in->getIndex());
	}
	else if (!out->isPolyphonic && in->isPolyphonic)
	{
		connectionType = PatchCableConnectionType::M2P;

		// only connect to the first voice of the poly input
		ac[0] = new AudioConnection(out->getLinkedStream(),
							out->getIndex(),
							in->getLinkedStream(0),
							in->getIndex());

		// alternatively, connect to all voices of the poly input
		// for (uint_fast8_t i = 0; i < POLYPHONY; ++i)
		// 	ac[i] = new AudioConnection(out->getLinkedStream(),
		// 								out->getIndex(),
		// 								in->getLinkedStream(),
		// 								i);
	}
	else if (out->isPolyphonic && !in->isPolyphonic)
	{
		connectionType = PatchCableConnectionType::P2M;

		ac[0] = new AudioConnection(out->getLinkedStream(0),
							out->getIndex(),
							in->getLinkedStream(),
							in->getIndex());

	}
	else if(out->isPolyphonic && in->isPolyphonic)
	{
		connectionType = PatchCableConnectionType::P2P;
		for (uint_fast8_t i = 0; i < POLYPHONY; ++i)
			ac[i] = new AudioConnection(out->getLinkedStream(i),
										out->getIndex(),
										in->getLinkedStream(i),
										in->getIndex());
	}
	// end

	this->inputSocket = in;
	this->outputSocket = out;

	// in cannot accept any other connections until this one is deleted
	InputSocket::setBusy(in);

	Serial.print(out->getName());
	Serial.print(" >>> ");
	Serial.print(in->getName());
	switch (connectionType)
	{
	case M2M:
		Serial.println(" M2M");
		break;
	case M2P:
		Serial.println(" M2P");
		break;
	case P2M:
		Serial.println(" P2M");
		break;
	case P2P:
		Serial.println(" P2P");
	}
}

// dtor
PatchCable::~PatchCable()
{
	switch (connectionType)
	{
	case PatchCableConnectionType::M2M:
	case PatchCableConnectionType::M2P:
	case PatchCableConnectionType::P2M:
	{
		ac[0]->disconnect();
		delete ac[0];
		break;
	}
	case PatchCableConnectionType::P2P:
	{
		for (uint_fast8_t i = 0; i < POLYPHONY; ++i)
		{
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

void PatchCable::onInputSocketConnected(InputSocket_p i)
{
	InputSocket::setAvailable(i);

	searchForCablesToAdd();
}

void PatchCable::onOutputSocketConnected(OutputSocket_p o)
{
	OutputSocket::setAvailable(o);

	searchForCablesToAdd();
}

void PatchCable::searchForCablesToAdd()
{
	if (OutputSocket::availableOutputs.empty() || InputSocket::availableInputs.empty())
		return;

	// check connection for every possible combination (out,in)
	for (auto out = OutputSocket::availableOutputs.begin(), end = OutputSocket::availableOutputs.end(); out != end; ++out)
	{
		for (auto in = InputSocket::availableInputs.begin(), end2 = InputSocket::availableInputs.end(); in != end2; ++in)
		{
			if (checkConnection(*out, *in))
			{
				// if (out,in) are connected, instantiate a patchcable
				// post increment the iterator because *in will be removed from available inputs
				activeCables.push_back(std::make_unique<PatchCable>(*out, *(in++)));
			}
		}
	}
}

/// @brief checks if an output socket is connected to an input socket
bool PatchCable::checkConnection(OutputSocket_p out, InputSocket_p in)
{
	out->sendSignal();

	if (in->isReceiving())
	{
		out->resetSignal();
		return true;
	}

	// else
	out->resetSignal();
	return false;
}

// destroy the patchcable that was disconnected from the input socket
void PatchCable::onInputSocketDisconnected(InputSocket_p input)
{
	if (!activeCables.empty())
		for (auto cable = activeCables.begin(), end = activeCables.end(); cable != end; ++cable)
			if ((*cable)->inputSocket->uid == input->uid)
			{
				activeCables.erase(cable);
				break; // i have to destroy only one cable
			}

	InputSocket::setInactive(input);
}

// destroy the patchcable that was disconnected from the output socket
void PatchCable::onOutputSocketDisconnected(OutputSocket_p output)
{
	if (!activeCables.empty())
	{
		for (auto cable = activeCables.begin(), end = activeCables.end(); cable != end;)
		{
			if ((*cable)->outputSocket->uid == output->uid)
			{
				// the inputs that were connected to this output must be set available again
				InputSocket::setAvailable((*cable)->inputSocket);

				activeCables.erase(cable++);
			}
			else
				++cable;
		}
	}

	OutputSocket::setInactive(output);
}
