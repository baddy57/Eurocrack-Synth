#include "test_mode.h"
#include "test_display.h"
#include "test_reader.h"
#include "../core/hardware_cfg.h"
#include "../controls/module_type_id_mux.h"
#include <Arduino.h>

// Static member definitions
bool TestMode::_active = false;
Address TestMode::_currentSlot(0);
const ModuleTestConfig* TestMode::_config = nullptr;
uint32_t TestMode::_lastUpdate = 0;

void TestMode::enter() {
	TestDisplay::init();

	detectModule();

	if (_config == nullptr) {
		TestDisplay::showError("No module config found");
		_active = true;  // Still active but showing error
		return;
	}

	// Draw static UI
	TestDisplay::drawHeader(_config->moduleName, _currentSlot.toInt(), _config->moduleTypeId);
	TestDisplay::drawAnalogSection(_config);
	TestDisplay::drawDigitalSection(_config);

	_active = true;
	_lastUpdate = millis();

	// Initial poll to show current values
	pollAnalogControls();
	pollDigitalControls();
}

void TestMode::update() {
	if (!_active || _config == nullptr) return;

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
	// Scan slots for first connected module
	for (uint8_t slot = 0; slot < MAX_MODULES; ++slot) {
		Address addr(slot);
		uint8_t typeId = ModuleTypeIdMux(addr).getModuleId();

		// Skip empty slots (0) and invalid (255)
		if (typeId != 0 && typeId != 255) {
			_currentSlot = addr;
			_config = findTestConfig(typeId);

			if (_config == nullptr) {
				// Module detected but no test config available
				// Still show basic info
				TestDisplay::drawHeader("UNKNOWN", slot, typeId);
			}
			return;
		}
	}

	// No module found
	TestDisplay::showError("No module detected!");
}

void TestMode::pollAnalogControls() {
	if (_config == nullptr || _config->numAnalog == 0) return;

	for (uint8_t i = 0; i < _config->numAnalog; ++i) {
		const TestControl& ctrl = _config->analogControls[i];
		uint16_t raw = TestReader::readAnalog(_currentSlot, ctrl.pinId);
		TestDisplay::updateAnalog(i, raw, ctrl);
	}
}

void TestMode::pollDigitalControls() {
	if (_config == nullptr || _config->numDigital == 0) return;

	for (uint8_t i = 0; i < _config->numDigital; ++i) {
		const TestControl& ctrl = _config->digitalControls[i];
		bool state = TestReader::readDigital(_currentSlot, ctrl.pinId, ctrl.type);
		TestDisplay::updateDigital(i, state, ctrl.type);
	}
}
