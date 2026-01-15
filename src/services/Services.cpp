// Static member definitions for all services
// This file exists because C++11 doesn't support inline static

#include "SynthDisplay.h"
#include "SynthAudio.h"
#include "Modules.h"
#include "Connections.h"
#include "../core/Module.h"
#include "../core/PatchCable.h"

// SynthDisplay
ILI9341_t3 SynthDisplay::_tft = ILI9341_t3(TFT_CS, TFT_DC);

void SynthDisplay::init() {
	_tft.begin();
	_tft.setRotation(4);
	_tft.fillScreen(CONFIGURATION__BACKGROUND_COLOR);
}

void SynthDisplay::print(const char* text) { _tft.print(text); }
void SynthDisplay::println(const char* text) { _tft.println(text); }
void SynthDisplay::print(int value) { _tft.print(value); }
void SynthDisplay::println(int value) { _tft.println(value); }
void SynthDisplay::fillScreen(uint16_t color) { _tft.fillScreen(color); }

// SynthAudio
AudioControlSGTL5000 SynthAudio::codec;

void SynthAudio::enable() {
	codec.enable();
}

void SynthAudio::setVolume(float vol) {
	codec.volume(vol);
}

// Modules
std::vector<Module*> Modules::active;

void Modules::updateAll() {
	for (auto* module : active) {
		module->updateValues();
	}
}

// Connections
// For now, delegates to existing static methods in InputSocket/OutputSocket/PatchCable
// Will be consolidated in later commits

std::vector<std::shared_ptr<InputSocket>> Connections::inputs;
std::vector<std::shared_ptr<OutputSocket>> Connections::outputs;
std::list<std::shared_ptr<InputSocket>> Connections::availableInputs;
std::list<std::shared_ptr<InputSocket>> Connections::busyInputs;
std::list<std::shared_ptr<OutputSocket>> Connections::availableOutputs;
std::list<std::unique_ptr<PatchCable>> Connections::_cables;

void Connections::registerInput(std::shared_ptr<InputSocket> socket) {
	inputs.push_back(socket);
}

void Connections::registerOutput(std::shared_ptr<OutputSocket> socket) {
	outputs.push_back(socket);
}

void Connections::update() {
	// Poll all inputs for jack events
	for (auto& inputSocket : Module::inputSockets) {
		if (inputSocket->jackJustPlugged())
			PatchCable::onInputSocketConnected(inputSocket);
		if (inputSocket->jackJustUnplugged())
			PatchCable::onInputSocketDisconnected(inputSocket);
	}

	// Poll all outputs for jack events
	for (auto& outputSocket : Module::outputSockets) {
		if (outputSocket->jackJustPlugged())
			PatchCable::onOutputSocketConnected(outputSocket);
		if (outputSocket->jackJustUnplugged())
			PatchCable::onOutputSocketDisconnected(outputSocket);
	}
}

// Delegate to existing methods (will be consolidated later)
void Connections::onInputConnected(std::shared_ptr<InputSocket> socket) {
	PatchCable::onInputSocketConnected(socket);
}
void Connections::onInputDisconnected(std::shared_ptr<InputSocket> socket) {
	PatchCable::onInputSocketDisconnected(socket);
}
void Connections::onOutputConnected(std::shared_ptr<OutputSocket> socket) {
	PatchCable::onOutputSocketConnected(socket);
}
void Connections::onOutputDisconnected(std::shared_ptr<OutputSocket> socket) {
	PatchCable::onOutputSocketDisconnected(socket);
}
void Connections::setInputAvailable(std::shared_ptr<InputSocket> socket) {
	InputSocket::setAvailable(socket);
}
void Connections::setInputBusy(std::shared_ptr<InputSocket> socket) {
	InputSocket::setBusy(socket);
}
void Connections::setInputInactive(std::shared_ptr<InputSocket> socket) {
	InputSocket::setInactive(socket);
}
void Connections::setOutputAvailable(std::shared_ptr<OutputSocket> socket) {
	OutputSocket::setAvailable(socket);
}
void Connections::setOutputInactive(std::shared_ptr<OutputSocket> socket) {
	OutputSocket::setInactive(socket);
}
void Connections::searchForCablesToAdd() {
	// Called internally by PatchCable - no external delegation needed
}
