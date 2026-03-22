#pragma once

#include <XPT2046_Touchscreen.h>
#include "../hardware_setup/motherboard.h"

struct TouchPoint {
	uint16_t x;
	uint16_t y;
	uint16_t z;  // pressure
};

struct TouchPointRaw {
	uint16_t x;
	uint16_t y;
	uint16_t z;
};

class SynthTouch {
	static XPT2046_Touchscreen _ts;
	static TouchPoint _currentPoint;
	static TouchPointRaw _currentPointRaw;
	static bool _wasTouched;
	static uint32_t _lastPollTime;

	// Calibration constants for coordinate mapping
	static constexpr uint16_t TS_MIN_X = 150;
	static constexpr uint16_t TS_MAX_X = 920;
	static constexpr uint16_t TS_MIN_Y = 120;
	static constexpr uint16_t TS_MAX_Y = 940;
	static constexpr uint16_t DISPLAY_WIDTH = 240;
	static constexpr uint16_t DISPLAY_HEIGHT = 320;
	static constexpr uint16_t POLL_INTERVAL_MS = 20;  // 50Hz

	static uint16_t mapCoordinate(uint16_t raw, uint16_t rawMin, uint16_t rawMax, uint16_t displayMax);

public:
	static void init();
	static void update();  // Non-blocking, call in loop()

	static bool isTouched();
	static TouchPoint getPoint();
	static TouchPointRaw getPointRaw();  // Get raw coordinates for calibration
	static bool justPressed();   // Rising edge
	static bool justReleased();  // Falling edge

	// Hit testing
	static bool isTouchedInRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	// Escape hatch for direct access when needed
	static XPT2046_Touchscreen& raw() { return _ts; }
};
