#include "synth_touch.h"

// Static member definitions
XPT2046_Touchscreen SynthTouch::_ts = XPT2046_Touchscreen(pins::TOUCH_CS, pins::TOUCH_IRQ);
TouchPoint SynthTouch::_currentPoint = {0, 0, 0};
bool SynthTouch::_wasTouched = false;
uint32_t SynthTouch::_lastPollTime = 0;

void SynthTouch::init() {
	_ts.begin();
	_ts.setRotation(1);  // Match display rotation
}

void SynthTouch::update() {
	// Non-blocking 50Hz polling
	uint32_t now = millis();
	if (now - _lastPollTime < POLL_INTERVAL_MS) {
		return;
	}
	_lastPollTime = now;

	bool currentlyTouched = _ts.touched();

	if (currentlyTouched) {
		TS_Point p = _ts.getPoint();

		// Map raw coordinates to display coordinates
		_currentPoint.x = mapCoordinate(p.x, TS_MIN_X, TS_MAX_X, DISPLAY_WIDTH);
		_currentPoint.y = mapCoordinate(p.y, TS_MIN_Y, TS_MAX_Y, DISPLAY_HEIGHT);
		_currentPoint.z = p.z;
	} else {
		_currentPoint.z = 0;
	}

	_wasTouched = currentlyTouched;
}

bool SynthTouch::isTouched() {
	return _wasTouched;
}

TouchPoint SynthTouch::getPoint() {
	return _currentPoint;
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
	// Clamp to valid range
	if (raw < rawMin) raw = rawMin;
	if (raw > rawMax) raw = rawMax;

	// Map to display coordinates
	uint32_t mapped = ((uint32_t)(raw - rawMin) * displayMax) / (rawMax - rawMin);

	// Clamp to display bounds
	if (mapped >= displayMax) mapped = displayMax - 1;

	return (uint16_t)mapped;
}
