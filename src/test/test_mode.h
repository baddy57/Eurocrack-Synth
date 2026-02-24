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
			TestDisplay::drawHeader("UNKNOWN", _currentSlot.getId(), _detectedTypeId);
			return;
		}

		// Get test controls from the module and separate them
		_analogControls.clear();
		_digitalControls.clear();
		_sockets.clear();

		_module->getTestControls(_analogControls, _digitalControls);
		_module->getTestSockets(_sockets);

		// Draw static UI
		TestDisplay::drawHeader(_module->getModuleName(), _currentSlot.getId(), _module->getModuleTypeId());
		TestDisplay::drawAnalogSection(_analogControls);
		TestDisplay::drawDigitalSection(_digitalControls);
		TestDisplay::drawSocketSection(_sockets);

		_lastUpdate = millis();
		_lastSocketToggle = millis();

		// Find first output socket
		_currentOutputSocketIndex = 0;
		while (_currentOutputSocketIndex < _sockets.size() && !_sockets[_currentOutputSocketIndex].isOutput) {
			_currentOutputSocketIndex++;
		}
		// Activate first output socket if found
		if (_currentOutputSocketIndex < _sockets.size() && _sockets[_currentOutputSocketIndex].outputSocket) {
			_sockets[_currentOutputSocketIndex].outputSocket->sendSignal();
		}

		// Initial poll to show current values
		pollAnalogControls();
		pollDigitalControls();
		pollSockets();
	}

	// Main loop update - polls controls and updates display
	static inline void update() {
		if (_module == nullptr) return;

		uint32_t now = millis();
		if (now - _lastUpdate < UPDATE_INTERVAL_MS) return;
		_lastUpdate = now;

		pollAnalogControls();
		pollDigitalControls();
		pollSockets();
	}

private:
	static inline Address _currentSlot{0};
	static inline Module* _module = nullptr;
	static inline uint8_t _detectedTypeId = 0;
	static inline std::vector<TestControlInfo> _analogControls;
	static inline std::vector<TestControlInfo> _digitalControls;
	static inline std::vector<TestSocketInfo> _sockets;
	static inline uint32_t _lastUpdate = 0;
	static inline uint32_t _lastSocketToggle = 0;
	static inline uint8_t _currentOutputSocketIndex = 0;

	// Update rate (20 Hz = 50ms interval)
	static constexpr uint32_t UPDATE_INTERVAL_MS = 50;
	// Socket toggle rate (1 second per output)
	static constexpr uint32_t SOCKET_TOGGLE_INTERVAL_MS = 1000;

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

	static inline void pollSockets() {
		if (_sockets.empty()) return;

		//poll jack detectors
		for (uint8_t i = 0; i < _sockets.size(); ++i) {
			const TestControlInfo& ctrl = _sockets[i].detector;
			bool state = TestReader::readDigital(_currentSlot, ctrl.pinId, ctrl.type);
			TestDisplay::updateJackDetector(i, state);
		}

		uint32_t now = millis();

		// Cycle through output sockets one at a time, 1 second each
		if (now - _lastSocketToggle >= SOCKET_TOGGLE_INTERVAL_MS) {
			_lastSocketToggle = now;

			// Reset current output socket
			if (_currentOutputSocketIndex < _sockets.size()) {
				const auto& currentSocket = _sockets[_currentOutputSocketIndex];
				if (currentSocket.isOutput && currentSocket.outputSocket) {
					currentSocket.outputSocket->resetSignal();
				}
			}

			// Move to next output socket
			_currentOutputSocketIndex++;

			// Find next output socket (skip input sockets)
			while (_currentOutputSocketIndex < _sockets.size() && !_sockets[_currentOutputSocketIndex].isOutput) {
				_currentOutputSocketIndex++;
			}

			// Wrap around to first output socket
			if (_currentOutputSocketIndex >= _sockets.size()) {
				_currentOutputSocketIndex = 0;
				// Find first output socket
				while (_currentOutputSocketIndex < _sockets.size() && !_sockets[_currentOutputSocketIndex].isOutput) {
					_currentOutputSocketIndex++;
				}
			}

			// Activate current output socket
			if (_currentOutputSocketIndex < _sockets.size()) {
				const auto& currentSocket = _sockets[_currentOutputSocketIndex];
				if (currentSocket.isOutput && currentSocket.outputSocket) {
					currentSocket.outputSocket->sendSignal();
				}
			}
		}

		// Update display for all sockets
		for (uint8_t i = 0; i < _sockets.size(); ++i) {
			const TestSocketInfo& socket = _sockets[i];
			if (socket.isOutput) {
				// Output socket - show TX only for currently active socket
				bool isActive = (i == _currentOutputSocketIndex);
				TestDisplay::updateJackSending(i, isActive);
			} else if (socket.inputSocket) {
				// Input socket - check if receiving
				bool receiving = socket.inputSocket->isReceiving();
				TestDisplay::updateJackReceiving(i, receiving);
			}
		}
	}
};
