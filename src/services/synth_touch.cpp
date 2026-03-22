#include "synth_touch.h"
#include <SPI.h>

// Static member definitions
XPT2046_Touchscreen SynthTouch::_ts = XPT2046_Touchscreen(pins::TOUCH_CS, pins::TOUCH_IRQ);
TouchPoint SynthTouch::_currentPoint = {0, 0, 0};
TouchPointRaw SynthTouch::_currentPointRaw = {0, 0, 0};
bool SynthTouch::_wasTouched = false;
uint32_t SynthTouch::_lastPollTime = 0;

void SynthTouch::init() {
	// Pin modes now handled by Motherboard::init()
	// SPI already initialized by display

	_ts.begin();
	_ts.setRotation(0);  // Match display rotation

	// Give it time to stabilize
	delay(10);
}

void SynthTouch::update() {
	// Non-blocking 50Hz polling
	uint32_t now = millis();
	if (now - _lastPollTime < POLL_INTERVAL_MS) {
		return;
	}
	_lastPollTime = now;

	bool currentlyTouched = false;

	// Check for touch (library handles SPI transaction and CS pin)
	if (_ts.touched()) {
		TS_Point p = _ts.getPoint();

		// Filter by pressure to eliminate noise
		// Valid touches have pressure in range [PRESSURE_MIN, PRESSURE_MAX]
		if (p.z >= PRESSURE_MIN && p.z <= PRESSURE_MAX) {
			currentlyTouched = true;

			// Store raw coordinates for debugging (only for valid touches)
			_currentPointRaw.x = p.x;
			_currentPointRaw.y = p.y;
			_currentPointRaw.z = p.z;

			// Map raw coordinates to display coordinates
			uint16_t mappedX = mapCoordinate(p.x, TS_MIN_X, TS_MAX_X, DISPLAY_WIDTH);
			uint16_t mappedY = mapCoordinate(p.y, TS_MIN_Y, TS_MAX_Y, DISPLAY_HEIGHT);

			// Extra safety: clamp to display bounds (should already be done in mapCoordinate)
			_currentPoint.x = (mappedX < DISPLAY_WIDTH) ? mappedX : (DISPLAY_WIDTH - 1);
			_currentPoint.y = (mappedY < DISPLAY_HEIGHT) ? mappedY : (DISPLAY_HEIGHT - 1);
			_currentPoint.z = p.z;
		}
	}

	if (!currentlyTouched) {
		_currentPoint.z = 0;
		_currentPointRaw.z = 0;
	}

	_wasTouched = currentlyTouched;
}

bool SynthTouch::isTouched() {
	return _wasTouched;
}

TouchPoint SynthTouch::getPoint() {
	return _currentPoint;
}

TouchPointRaw SynthTouch::getPointRaw() {
	return _currentPointRaw;
}

bool SynthTouch::justPressed() {
	static bool lastState = false;
	bool pressed = !lastState && _wasTouched;
	lastState = _wasTouched;
	return pressed;
}

bool SynthTouch::justReleased() {
	static bool lastState = false;
	bool released = lastState && !_wasTouched;
	lastState = _wasTouched;
	return released;
}

bool SynthTouch::isTouchedInRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	if (!_wasTouched) return false;

	return _currentPoint.x >= x &&
	       _currentPoint.x < (x + w) &&
	       _currentPoint.y >= y &&
	       _currentPoint.y < (y + h);
}

uint16_t SynthTouch::mapCoordinate(uint16_t raw, uint16_t rawMin, uint16_t rawMax, uint16_t displayMax) {
	// Safety: prevent division by zero
	if (rawMax <= rawMin) return 0;
	if (displayMax == 0) return 0;

	// Clamp to valid range
	if (raw < rawMin) raw = rawMin;
	if (raw > rawMax) raw = rawMax;

	// Map to display coordinates (use 32-bit to prevent overflow)
	uint32_t mapped = ((uint32_t)(raw - rawMin) * displayMax) / (rawMax - rawMin);

	// Clamp to display bounds (ensure we never exceed displayMax)
	if (mapped >= displayMax) mapped = displayMax - 1;

	return (uint16_t)mapped;
}
