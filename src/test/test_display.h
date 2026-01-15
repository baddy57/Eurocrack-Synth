#pragma once

#include <cstdint>
#include "test_config.h"

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

class TestDisplay {
public:
	static void init();
	static void drawHeader(const char* moduleName, uint8_t slot, uint8_t typeId);
	static void drawAnalogSection(const ModuleTestConfig* config);
	static void drawDigitalSection(const ModuleTestConfig* config);

	// In-place value updates (no full redraw)
	static void updateAnalog(uint8_t row, uint16_t raw, const TestControl& ctrl);
	static void updateDigital(uint8_t row, bool state, TestControlType type);

	static void showError(const char* message);

private:
	static uint16_t getAnalogColor(uint16_t raw);

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
	static constexpr uint8_t COL_PIN = 60;
	static constexpr uint8_t COL_RAW = 90;
	static constexpr uint8_t COL_PROCESSED = 140;
	static constexpr uint8_t COL_VALUE = 90;  // For digital section

	// Helper to calculate processed value from raw
	static float calculateProcessed(uint16_t raw, float minVal, float maxVal);
};
