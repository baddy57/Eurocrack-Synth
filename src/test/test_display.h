#pragma once

#include <cstdint>
#include <vector>
#include "test_config.h"
#include "../services/synth_display.h"
#include <ILI9341_t3.h>

// Colors for test display
#define TEST_COLOR_BACKGROUND   ILI9341_BLACK
#define TEST_COLOR_HEADER       ILI9341_CYAN
#define TEST_COLOR_LABEL        ILI9341_WHITE
#define TEST_COLOR_VALUE_NORMAL ILI9341_GREEN
#define TEST_COLOR_VALUE_ZERO   ILI9341_YELLOW
#define TEST_COLOR_VALUE_MAX    ILI9341_RED
#define TEST_COLOR_DIGITAL_ON   ILI9341_GREEN
#define TEST_COLOR_DIGITAL_OFF  0x4208  // Dark gray
#define TEST_COLOR_JACK_PRESENT ILI9341_MAGENTA
#define TEST_COLOR_SEPARATOR    0x4208  // Dark gray

#define tft SynthDisplay::raw()

class TestDisplay {
public:
	static inline void init() {
		tft.fillScreen(TEST_COLOR_BACKGROUND);
		tft.setTextSize(1);
	}

	static inline void drawHeader(const char* moduleName, uint8_t slot, uint8_t typeId) {
		// Header line
		tft.setTextColor(TEST_COLOR_HEADER);
		tft.setCursor(COL_NAME, HEADER_Y);
		tft.print("TEST MODE - ");
		tft.println(moduleName);

		// Info line
		tft.setTextColor(TEST_COLOR_LABEL);
		tft.setCursor(COL_NAME, INFO_Y);
		tft.print("Slot: ");
		tft.print(slot);
		tft.print("   Type: ");
		// Print all 8 binary digits with leading zeros
		for (int8_t i = 7; i >= 0; --i) {
			tft.print((typeId >> i) & 1);
		}
	}

	static inline void drawAnalogSection(const std::vector<TestControlInfo>& controls) {
		// Section header
		tft.setTextColor(TEST_COLOR_SEPARATOR);
		tft.setCursor(COL_NAME, ANALOG_HEADER_Y);
		tft.print("ANALOG     PIN  RAW   VALUE");

		// Draw labels for each analog control
		tft.setTextColor(TEST_COLOR_LABEL);
		for (uint8_t i = 0; i < controls.size(); ++i) {
			uint8_t y = ANALOG_START_Y + (i * ROW_HEIGHT);
			tft.setCursor(COL_NAME, y);
			tft.print(controls[i].name);
			tft.setCursor(COL_PIN, y);
			tft.print(controls[i].pinId);
		}
	}

	static inline void drawDigitalSection(const std::vector<TestControlInfo>& controls) {
		// Section header
		tft.setTextColor(TEST_COLOR_SEPARATOR);
		tft.setCursor(COL_NAME, DIGITAL_HEADER_Y);
		tft.print("DIGITAL    PIN  STATE");

		// Draw labels for each digital control
		tft.setTextColor(TEST_COLOR_LABEL);
		for (uint8_t i = 0; i < controls.size(); ++i) {
			uint8_t y = DIGITAL_START_Y + (i * ROW_HEIGHT);
			tft.setCursor(COL_NAME, y);
			tft.print(controls[i].name);
			tft.setCursor(COL_PIN, y);
			tft.print(controls[i].pinId);
		}
	}

	// In-place value updates (no full redraw)
	static inline void updateAnalog(uint8_t row, uint16_t raw) {
		uint8_t y = ANALOG_START_Y + (row * ROW_HEIGHT);

		// Clear raw value area
		tft.fillRect(COL_RAW, y, 60, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw raw value with color coding
		tft.setCursor(COL_RAW, y);
		tft.setTextColor(getAnalogColor(raw));
		tft.print(raw);
	}

	static inline void updateDigital(uint8_t row, bool state, TestControlType type) {
		uint8_t y = DIGITAL_START_Y + (row * ROW_HEIGHT);

		// Clear state area
		tft.fillRect(COL_VALUE, y, 60, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw state with appropriate color and label
		tft.setCursor(COL_VALUE, y);

		if (type == TestControlType::JACK_DETECTOR) {
			if (state) {
				tft.setTextColor(TEST_COLOR_JACK_PRESENT);
				tft.print("[JACK]");
			} else {
				tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
				tft.print("[----]");
			}
		} else {
			// BUTTON, SWITCH, or SELECTOR_MULTI
			if (state) {
				tft.setTextColor(TEST_COLOR_DIGITAL_ON);
				tft.print("[ ON ]");
			} else {
				tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
				tft.print("[OFF ]");
			}
		}
	}

	static inline void showError(const char* message) {
		tft.fillScreen(TEST_COLOR_BACKGROUND);
		tft.setTextSize(2);
		tft.setTextColor(ILI9341_RED);
		tft.setCursor(20, 100);
		tft.println("ERROR");
		tft.setTextSize(1);
		tft.setTextColor(TEST_COLOR_LABEL);
		tft.setCursor(20, 130);
		tft.println(message);
	}

private:
	static inline uint16_t getAnalogColor(uint16_t raw) {
		if (raw < 10) return TEST_COLOR_VALUE_ZERO;
		if (raw > 1013) return TEST_COLOR_VALUE_MAX;
		return TEST_COLOR_VALUE_NORMAL;
	}

	// Screen layout constants
	static constexpr uint8_t HEADER_Y = 0;
	static constexpr uint8_t INFO_Y = 16;
	static constexpr uint8_t ANALOG_HEADER_Y = 36;
	static constexpr uint8_t ANALOG_START_Y = 52;
	static constexpr uint8_t DIGITAL_HEADER_Y = 140;
	static constexpr uint8_t DIGITAL_START_Y = 156;
	static constexpr uint8_t ROW_HEIGHT = 12;

	// Column positions
	static constexpr uint8_t COL_NAME = 4;
	static constexpr uint8_t COL_PIN = 70;
	static constexpr uint8_t COL_RAW = 100;
	static constexpr uint8_t COL_VALUE = 100;  // For digital section
};

#undef tft
