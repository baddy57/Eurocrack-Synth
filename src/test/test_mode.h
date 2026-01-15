#pragma once

#include <cstdint>
#include <vector>
#include "../core/address.h"
#include "test_config.h"

class Module;  // Forward declaration

class TestMode {
public:
	// Enter test mode - detects module and draws initial UI
	static void enter();

	// Main loop update - polls controls and updates display
	static void update();

	// Check if test mode is active
	static bool isActive();

private:
	static bool _active;
	static Address _currentSlot;
	static Module* _module;
	static uint8_t _detectedTypeId;  // Store type ID even when module creation fails
	static std::vector<TestControlInfo> _analogControls;
	static std::vector<TestControlInfo> _digitalControls;
	static uint32_t _lastUpdate;

	// Detect first connected module
	static void detectModule();

	// Poll all controls and update display
	static void pollAnalogControls();
	static void pollDigitalControls();

	// Update rate (20 Hz = 50ms interval)
	static constexpr uint32_t UPDATE_INTERVAL_MS = 50;
};
