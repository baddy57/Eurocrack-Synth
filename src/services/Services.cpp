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

// Connections - stub implementations (will be filled in Phase 5)
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
	// Stub - will be implemented in Phase 5
}

void Connections::onInputConnected(std::shared_ptr<InputSocket> socket) {}
void Connections::onInputDisconnected(std::shared_ptr<InputSocket> socket) {}
void Connections::onOutputConnected(std::shared_ptr<OutputSocket> socket) {}
void Connections::onOutputDisconnected(std::shared_ptr<OutputSocket> socket) {}
void Connections::setInputAvailable(std::shared_ptr<InputSocket> socket) {}
void Connections::setInputBusy(std::shared_ptr<InputSocket> socket) {}
void Connections::setInputInactive(std::shared_ptr<InputSocket> socket) {}
void Connections::setOutputAvailable(std::shared_ptr<OutputSocket> socket) {}
void Connections::setOutputInactive(std::shared_ptr<OutputSocket> socket) {}
void Connections::searchForCablesToAdd() {}
