#include "synth_display.h"

// Static member definition
ILI9341_t3 SynthDisplay::_tft = ILI9341_t3(pins::LCD_CS, pins::LCD_DC);

void SynthDisplay::init() {
	_tft.begin();
	_tft.setRotation(0);
	_tft.fillScreen(CONFIGURATION__BACKGROUND_COLOR);
}

void SynthDisplay::print(const char* text) { _tft.print(text); }
void SynthDisplay::println(const char* text) { _tft.println(text); }
void SynthDisplay::print(int value) { _tft.print(value); }
void SynthDisplay::println(int value) { _tft.println(value); }
void SynthDisplay::fillScreen(uint16_t color) { _tft.fillScreen(color); }
