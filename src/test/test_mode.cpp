#include "test_mode.h"
#include "test_display.h"
#include "test_reader.h"
#include "../core/hardware_cfg.h"
#include "../controls/module_type_id_mux.h"
#include "../core/module_type_ids.h"
#include <Arduino.h>

// Include all module headers for instantiation
#include "../modules/empty_module.h"
#include "../modules/audio_out.h"
#include "../modules/audio_in.h"
#include "../modules/VCA.h"
#include "../modules/lfo.h"
#include "../modules/VCO_det.h"
#include "../modules/mixer8.h"
#include "../modules/midi_mono.h"
#include "../modules/drum_machine.h"
#include "../modules/Vcf.h"
#include "../modules/ADSR.h"
#include "../modules/Looper.h"
#include "../modules/Reverb.h"
#include "../modules/Distortion_amp.h"
#include "../modules/Distortion_bc.h"
#include "../modules/Delay_single.h"
#include "../modules/Delay_multi.h"

// Static member definitions
bool TestMode::_active = false;
Address TestMode::_currentSlot(0);
Module* TestMode::_module = nullptr;
uint8_t TestMode::_detectedTypeId = 0;
std::vector<TestControlInfo> TestMode::_analogControls;
std::vector<TestControlInfo> TestMode::_digitalControls;
uint32_t TestMode::_lastUpdate = 0;

// Helper to create module by type ID
static Module* createModuleByType(uint8_t typeId, const Address& addr) {
	switch (typeId) {
		case ModuleTypeIds::AUDIO_OUT: return new AudioOut(addr);
		case ModuleTypeIds::AUDIO_IN: return new AudioIn(addr);
		case ModuleTypeIds::FILTER: return new VCF(addr);
		case ModuleTypeIds::VCO_ID: return new VCO_det(addr);
		case ModuleTypeIds::MIDI_MODULE: return new MidiMono(addr);
		case ModuleTypeIds::ADSR: return new ADSR(addr);
		case ModuleTypeIds::MIXER8: return new Mixer8(addr);
		case ModuleTypeIds::VCA: return new VCA(addr);
		case ModuleTypeIds::DRUMS: return new DrumMachine(addr);
		case ModuleTypeIds::DELAY: return new Delay_multi(addr);
		case ModuleTypeIds::DISTORTION: return new Distortion_bc(addr);
		case ModuleTypeIds::REVERB: return new Reverb(addr);
		case ModuleTypeIds::DISTORTION_AMP: return new Distortion_amp(addr);
		case ModuleTypeIds::DELAY_SINGLE: return new Delay_single(addr);
		case ModuleTypeIds::LOOPER: return new Looper(addr);
		case ModuleTypeIds::LFO: return new LFO(addr);
		default: return nullptr;
	}
}

void TestMode::enter() {
	TestDisplay::init();

	detectModule();

	if (_detectedTypeId == 0) {
		// No module detected at all
		TestDisplay::showError("No module detected!");
		_active = true;
		return;
	}

	if (_module == nullptr) {
		// Module detected but no implementation - show type ID anyway
		TestDisplay::drawHeader("UNKNOWN", _currentSlot.toInt(), _detectedTypeId);
		_active = true;
		return;
	}

	// Get test controls from the module
	_analogControls.clear();
	_digitalControls.clear();
	_module->getTestControls(_analogControls, _digitalControls);

	// Draw static UI
	TestDisplay::drawHeader(_module->getModuleName(), _currentSlot.toInt(), _module->getModuleTypeId());
	TestDisplay::drawAnalogSection(_analogControls);
	TestDisplay::drawDigitalSection(_digitalControls);

	_active = true;
	_lastUpdate = millis();

	// Initial poll to show current values
	pollAnalogControls();
	pollDigitalControls();
}

void TestMode::update() {
	if (!_active || _module == nullptr) return;

	uint32_t now = millis();
	if (now - _lastUpdate < UPDATE_INTERVAL_MS) return;
	_lastUpdate = now;

	pollAnalogControls();
	pollDigitalControls();
}

bool TestMode::isActive() {
	return _active;
}

void TestMode::detectModule() {
	_detectedTypeId = 0;  // Reset

	// Scan slots for first connected module
	for (uint8_t slot = 0; slot < MAX_MODULES; ++slot) {
		Address addr(slot);
		uint8_t typeId = ModuleTypeIdMux(addr).getModuleId();

		// Skip empty slots (0) and invalid (255)
		if (typeId != 0 && typeId != 255) {
			_currentSlot = addr;
			_detectedTypeId = typeId;  // Store the detected type ID
			_module = createModuleByType(typeId, addr);
			return;
		}
	}
}

void TestMode::pollAnalogControls() {
	if (_analogControls.empty()) return;

	for (uint8_t i = 0; i < _analogControls.size(); ++i) {
		const TestControlInfo& ctrl = _analogControls[i];
		uint16_t raw = TestReader::readAnalog(_currentSlot, ctrl.pinId);
		TestDisplay::updateAnalog(i, raw);
	}
}

void TestMode::pollDigitalControls() {
	if (_digitalControls.empty()) return;

	for (uint8_t i = 0; i < _digitalControls.size(); ++i) {
		const TestControlInfo& ctrl = _digitalControls[i];
		bool state = TestReader::readDigital(_currentSlot, ctrl.pinId, ctrl.type);
		TestDisplay::updateDigital(i, state, ctrl.type);
	}
}
