// Static member definitions for all services
// This file exists because C++11 doesn't support inline static

#include "synth_display.h"
#include "synth_audio.h"
#include "module_manager.h"
#include "patch_cable_manager.h"
#include "../core/module.h"
#include "../core/patch_cable.h"

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

// ModuleManager
std::vector<Module*> ModuleManager::activeModules;

void ModuleManager::updateAll() {
	for (auto* module : activeModules) {
		module->updateValues();
	}
}

// PatchCableManager - unified socket and cable management

std::vector<std::shared_ptr<InputSocket>> PatchCableManager::inputs;
std::vector<std::shared_ptr<OutputSocket>> PatchCableManager::outputs;
std::list<std::shared_ptr<InputSocket>> PatchCableManager::availableInputs;
std::list<std::shared_ptr<InputSocket>> PatchCableManager::busyInputs;
std::list<std::shared_ptr<OutputSocket>> PatchCableManager::availableOutputs;
std::list<std::unique_ptr<PatchCable>> PatchCableManager::_cables;

void PatchCableManager::registerInput(std::shared_ptr<InputSocket> socket) {
	inputs.push_back(socket);
}

void PatchCableManager::registerOutput(std::shared_ptr<OutputSocket> socket) {
	outputs.push_back(socket);
}

void PatchCableManager::update() {
	// Poll all inputs for jack events
	for (auto& inputSocket : inputs) {
		if (inputSocket->jackJustPlugged())
			onInputConnected(inputSocket);
		if (inputSocket->jackJustUnplugged())
			onInputDisconnected(inputSocket);
	}

	// Poll all outputs for jack events
	for (auto& outputSocket : outputs) {
		if (outputSocket->jackJustPlugged())
			onOutputConnected(outputSocket);
		if (outputSocket->jackJustUnplugged())
			onOutputDisconnected(outputSocket);
	}
}

// Helper functions for list management
static void removeInputFromAvailable(std::shared_ptr<InputSocket> i) {
	for (auto it = PatchCableManager::availableInputs.begin(); it != PatchCableManager::availableInputs.end(); ++it) {
		if ((*it)->uid == i->uid) {
			PatchCableManager::availableInputs.erase(it);
			return;
		}
	}
}

static void removeInputFromBusy(std::shared_ptr<InputSocket> i) {
	for (auto it = PatchCableManager::busyInputs.begin(); it != PatchCableManager::busyInputs.end(); ++it) {
		if ((*it)->uid == i->uid) {
			PatchCableManager::busyInputs.erase(it);
			return;
		}
	}
}

static void removeOutputFromAvailable(std::shared_ptr<OutputSocket> out) {
	for (auto it = PatchCableManager::availableOutputs.begin(); it != PatchCableManager::availableOutputs.end(); ++it) {
		if ((*it)->uid == out->uid) {
			PatchCableManager::availableOutputs.erase(it);
			return;
		}
	}
}

// State transitions
void PatchCableManager::setInputAvailable(std::shared_ptr<InputSocket> i) {
	availableInputs.push_back(i);
	if (i->state == SocketState::BUSY)
		removeInputFromBusy(i);
	i->state = SocketState::AVAILABLE;
}

void PatchCableManager::setInputBusy(std::shared_ptr<InputSocket> i) {
	busyInputs.push_back(i);
	if (i->state == SocketState::AVAILABLE)
		removeInputFromAvailable(i);
	i->state = SocketState::BUSY;
}

void PatchCableManager::setInputInactive(std::shared_ptr<InputSocket> i) {
	if (i->state == SocketState::AVAILABLE)
		removeInputFromAvailable(i);
	else if (i->state == SocketState::BUSY)
		removeInputFromBusy(i);
	i->state = SocketState::INACTIVE;
}

void PatchCableManager::setOutputAvailable(std::shared_ptr<OutputSocket> out) {
	availableOutputs.push_back(out);
	out->state = SocketState::AVAILABLE;
}

void PatchCableManager::setOutputInactive(std::shared_ptr<OutputSocket> out) {
	if (out->state == SocketState::AVAILABLE)
		removeOutputFromAvailable(out);
	out->state = SocketState::INACTIVE;
}

// Cable search - checks all available outputs against all available inputs
void PatchCableManager::searchForCablesToAdd() {
	if (availableOutputs.empty() || availableInputs.empty())
		return;

	for (auto out = availableOutputs.begin(); out != availableOutputs.end(); ++out) {
		for (auto in = availableInputs.begin(); in != availableInputs.end(); ++in) {
			if (PatchCable::checkConnection(*out, *in)) {
				// if (out,in) are connected, instantiate a patchcable
				// post increment the iterator because *in will be removed from available inputs
				_cables.push_back(std::make_unique<PatchCable>(*out, *(in++)));
			}
		}
	}
}

// Jack event handlers
void PatchCableManager::onInputConnected(std::shared_ptr<InputSocket> i) {
	#if CONFIGURATION__LOGGER__JACK_EVENTS
	SynthDisplay::raw().print("Input connected: ");
	SynthDisplay::raw().println(i->getName());
	#endif

	setInputAvailable(i);
	searchForCablesToAdd();
}

void PatchCableManager::onOutputConnected(std::shared_ptr<OutputSocket> o) {
	#if CONFIGURATION__LOGGER__JACK_EVENTS
	SynthDisplay::raw().print("Output connected: ");
	SynthDisplay::raw().println(o->getName());
	#endif

	setOutputAvailable(o);
	searchForCablesToAdd();
}

void PatchCableManager::onInputDisconnected(std::shared_ptr<InputSocket> input) {
	#if CONFIGURATION__LOGGER__JACK_EVENTS
	SynthDisplay::raw().print("Input disconnected: ");
	SynthDisplay::raw().println(input->getName());
	#endif

	// Find and destroy the cable connected to this input
	if (!_cables.empty()) {
		for (auto cable = _cables.begin(); cable != _cables.end(); ++cable) {
			if ((*cable)->inputSocket->uid == input->uid) {
				_cables.erase(cable);
				break;
			}
		}
	}

	setInputInactive(input);
}

void PatchCableManager::onOutputDisconnected(std::shared_ptr<OutputSocket> output) {
	#if CONFIGURATION__LOGGER__JACK_EVENTS
	SynthDisplay::raw().print("Output disconnected: ");
	SynthDisplay::raw().println(output->getName());
	#endif

	// Find and destroy all cables connected to this output
	if (!_cables.empty()) {
		for (auto cable = _cables.begin(); cable != _cables.end();) {
			if ((*cable)->outputSocket->uid == output->uid) {
				// Return connected inputs to available state
				setInputAvailable((*cable)->inputSocket);
				cable = _cables.erase(cable);
			} else {
				++cable;
			}
		}
	}

	setOutputInactive(output);
}
