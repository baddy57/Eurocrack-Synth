#include "patch_cable_connection_type.h"
#include "patch_cable.h"
#include <memory>
#include "../services/synth_display.h"
#include "../services/patch_cable_manager.h"

#define tft SynthDisplay::raw()

//ctor
PatchCable::PatchCable(std::shared_ptr<OutputSocket> out, std::shared_ptr<InputSocket> in)
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

	this->inputSocket = in;
	this->outputSocket = out;

	// in cannot accept any other connections until this one is deleted
	PatchCableManager::setInputBusy(in);

	#if CONFIGURATION__LOGGER__CONNECTIONS
	tft.print(out->getName());
	tft.print(" >>> ");
	tft.print(in->getName());
	switch (connectionType)
	{
	case M2M:
		tft.println(" M2M");
		break;
	case M2P:
		tft.println(" M2P");
		break;
	case P2M:
		tft.println(" P2M");
		break;
	case P2P:
		tft.println(" P2P");
	}
	#endif
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

	#if CONFIGURATION__LOGGER__CONNECTIONS
	tft.print(outputSocket->getName());
	tft.print(" XXX ");
	tft.println(inputSocket->getName());
	#endif
}

/// @brief checks if an output socket is connected to an input socket
bool PatchCable::checkConnection(std::shared_ptr<OutputSocket> out, std::shared_ptr<InputSocket> in)
{
	out->sendSignal();

	if (in->isReceiving())
	{
		out->resetSignal();
		return true;
	}

	out->resetSignal();
	return false;
}
