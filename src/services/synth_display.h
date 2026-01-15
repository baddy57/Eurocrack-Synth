#pragma once

#include <ILI9341_t3.h>
#include "../core/hardware_cfg.h"

class SynthDisplay {
	static ILI9341_t3 _tft;

public:
	static void init();
	static void print(const char* text);
	static void println(const char* text);
	static void print(int value);
	static void println(int value);
	static void fillScreen(uint16_t color);

	// Escape hatch for direct access when needed
	static ILI9341_t3& raw() { return _tft; }
};
