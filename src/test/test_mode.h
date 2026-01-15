#pragma once

#include <cstdint>
#include <vector>
#include "../core/address.h"
#include "../core/hardware_cfg.h"
#include "../controls/module_type_id_mux.h"
#include "test_config.h"
#include "test_display.h"
#include "test_reader.h"
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

class TestMode {
public:
	// Enter test mode - detects module and draws initial UI
	static inline void enter() {
		TestDisplay::init();

		detectModule();

		if (_detectedTypeId == 0) {
			// No module detected at all
			TestDisplay::showError("No module detected!");
			return;
		}

		if (_module == nullptr) {
			// Module detected but no implementation - show type ID anyway
			TestDisplay::drawHeader("UNKNOWN", _currentSlot.toInt(), _detectedTypeId);
			return;
		}

		// Get test controls from the module and separate them
		std::vector<TestControlInfo> allDigital;
		_analogControls.clear();
		allDigital.clear();
		_digitalControls.clear();
		_jackDetectors.clear();

		_module->getTestControls(_analogControls, allDigital);

		// Separate digital controls from jack detectors
		for (const auto& ctrl : allDigital) {
			if (ctrl.type == TestControlType::JACK_DETECTOR) {
				_jackDetectors.push_back(ctrl);
			} else {
				_digitalControls.push_back(ctrl);
			}
		}

		// Draw static UI
		TestDisplay::drawHeader(_module->getModuleName(), _currentSlot.toInt(), _module->getModuleTypeId());
		TestDisplay::drawAnalogSection(_analogControls);
		TestDisplay::drawDigitalSection(_digitalControls);
		TestDisplay::drawJackDetectorSection(_jackDetectors);

		_lastUpdate = millis();

		// Initial poll to show current values
		pollAnalogControls();
		pollDigitalControls();
		pollJackDetectors();
	}

	// Main loop update - polls controls and updates display
	static inline void update() {
		if (_module == nullptr) return;

		uint32_t now = millis();
		if (now - _lastUpdate < UPDATE_INTERVAL_MS) return;
		_lastUpdate = now;

		pollAnalogControls();
		pollDigitalControls();
		pollJackDetectors();
	}

private:
	static inline Address _currentSlot{0};
	static inline Module* _module = nullptr;
	static inline uint8_t _detectedTypeId = 0;
	static inline std::vector<TestControlInfo> _analogControls;
	static inline std::vector<TestControlInfo> _digitalControls;
	static inline std::vector<TestControlInfo> _jackDetectors;
	static inline uint32_t _lastUpdate = 0;

	// Update rate (20 Hz = 50ms interval)
	static constexpr uint32_t UPDATE_INTERVAL_MS = 50;

	// Helper to create module by type ID
	static inline Module* createModuleByType(uint8_t typeId, const Address& addr) {
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

	// Detect first connected module
	static inline void detectModule() {
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

	// Poll all controls and update display
	static inline void pollAnalogControls() {
		if (_analogControls.empty()) return;

		for (uint8_t i = 0; i < _analogControls.size(); ++i) {
			const TestControlInfo& ctrl = _analogControls[i];
			uint16_t raw = TestReader::readAnalog(_currentSlot, ctrl.pinId);
			TestDisplay::updateAnalog(i, raw, ctrl.pot);
		}
	}

	static inline void pollDigitalControls() {
		if (_digitalControls.empty()) return;

		for (uint8_t i = 0; i < _digitalControls.size(); ++i) {
			const TestControlInfo& ctrl = _digitalControls[i];
			bool state = TestReader::readDigital(_currentSlot, ctrl.pinId, ctrl.type);
			TestDisplay::updateDigital(i, state, ctrl.type);
		}
	}

	static inline void pollJackDetectors() {
		if (_jackDetectors.empty()) return;

		for (uint8_t i = 0; i < _jackDetectors.size(); ++i) {
			const TestControlInfo& ctrl = _jackDetectors[i];
			bool state = TestReader::readDigital(_currentSlot, ctrl.pinId, ctrl.type);
			TestDisplay::updateJackDetector(i, state);
		}
	}
};
