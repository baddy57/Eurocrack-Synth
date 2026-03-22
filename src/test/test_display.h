#pragma once

#include <cstdint>
#include <vector>
#include "test_config.h"
#include "../services/synth_display.h"
#include "../controls/potentiometer.h"
#include <ILI9341_t3.h>

// Colors for test display
#define TEST_COLOR_BACKGROUND   ILI9341_BLACK
#define TEST_COLOR_HEADER       ILI9341_CYAN
#define TEST_COLOR_LABEL        ILI9341_WHITE
#define TEST_COLOR_VALUE_NORMAL ILI9341_GREEN
#define TEST_COLOR_VALUE_ZERO   ILI9341_YELLOW
#define TEST_COLOR_VALUE_MAX    ILI9341_RED
#define TEST_COLOR_DIGITAL_ON   ILI9341_GREEN
#define TEST_COLOR_DIGITAL_OFF  0x8410  // Light gray
#define TEST_COLOR_JACK_PRESENT ILI9341_MAGENTA
#define TEST_COLOR_SEPARATOR    0x8410  // Light gray

#define tft SynthDisplay::raw()

#include "../controls/output_socket.h"
#include "../controls/input_socket.h"

class TestDisplay {
	
	private:
	// Screen layout constants
	static constexpr uint8_t HEADER_Y = 0;
	static constexpr uint8_t INFO_Y = 16;
	inline static uint8_t ANALOG_HEADER_Y = 36;
	inline static uint8_t ANALOG_START_Y = 52;
	inline static uint8_t DIGITAL_HEADER_Y;
	inline static uint8_t DIGITAL_START_Y;
	inline static uint8_t SOCKET_HEADER_Y;
	inline static uint8_t SOCKET_START_Y;
	static constexpr uint8_t ROW_HEIGHT = 12;
	
	// Column positions
	static constexpr uint8_t COL_NAME = 4;
	static constexpr uint8_t COL_PIN = 70;
	static constexpr uint8_t COL_RAW = 100;
	static constexpr uint8_t COL_COMPUTED = 140;
	static constexpr uint8_t COL_VALUE = 100;  // For digital section
	static constexpr uint8_t COL_DETECTOR = 112;  
	
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
		if(controls.empty()) {
			DIGITAL_HEADER_Y = ANALOG_HEADER_Y;
			DIGITAL_START_Y = ANALOG_START_Y;
			return;
		}

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
			tft.setCursor(COL_RAW, y);
			tft.print(controls[i].pot->read());
		}

		DIGITAL_HEADER_Y = ANALOG_START_Y + (controls.size() * ROW_HEIGHT) + 10;
		DIGITAL_START_Y = DIGITAL_HEADER_Y + ROW_HEIGHT + 4;
	}

	static inline void drawDigitalSection(const std::vector<TestControlInfo>& controls) {

		if(controls.empty()) {
			SOCKET_HEADER_Y = DIGITAL_HEADER_Y;
			SOCKET_START_Y = DIGITAL_START_Y;
			return;
		}

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

		SOCKET_HEADER_Y = DIGITAL_START_Y + (controls.size() * ROW_HEIGHT) + 10;
		SOCKET_START_Y = SOCKET_HEADER_Y + ROW_HEIGHT + 4;
	}

	static inline void drawSocketSection(const std::vector<TestSocketInfo>& sockets) {
		if (sockets.empty())
			return;

		// Section header
		tft.setTextColor(TEST_COLOR_SEPARATOR);
		tft.setCursor(COL_NAME, SOCKET_HEADER_Y);
		tft.print("JACKS      PIN(D) DETECT  DATA");

		// Draw labels for each jack detector
		tft.setTextColor(TEST_COLOR_LABEL);
		for (uint8_t i = 0; i < sockets.size(); ++i) {

			auto s = sockets[i];

			uint8_t y = SOCKET_START_Y + (i * ROW_HEIGHT);
			tft.setCursor(COL_NAME, y);
			tft.print(sockets[i].name);
			tft.setCursor(COL_PIN, y);
			tft.print(s.isOutput 
				? sockets[i].outputSocket->address->getModulePin() 
				: sockets[i].inputSocket->address->getModulePin()); // todo fixme arriva ilo pin sbagliato
			tft.print('(');
			tft.print(sockets[i].detector.pinId);
			tft.print(')');
		}
	}

	// In-place value updates (no full redraw)
	static inline void updateAnalog(uint8_t row, uint16_t raw, Potentiometer* pot) {
		uint8_t y = ANALOG_START_Y + (row * ROW_HEIGHT);

		// Clear raw and computed value areas
		tft.fillRect(COL_RAW, y, 130, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw raw value with color coding
		tft.setCursor(COL_RAW, y);
		tft.setTextColor(TEST_COLOR_VALUE_NORMAL);
		tft.print(raw);

		// Draw computed value if potentiometer available
		if (pot != nullptr) {
			tft.setCursor(COL_COMPUTED, y);
			tft.setTextColor(TEST_COLOR_LABEL);
			pot->update();
			float computed = pot->read();
			// Format based on value magnitude
			if (computed >= 100 || computed <= -100) {
				tft.print((int)computed);
			} else if (computed >= 10 || computed <= -10) {
				tft.print(computed, 1);
			} else {
				tft.print(computed, 2);
			}
		}
	}

	static inline void updateDigital(uint8_t row, bool state, TestControlType type) {
		uint8_t y = DIGITAL_START_Y + (row * ROW_HEIGHT);

		// Clear state area
		tft.fillRect(COL_VALUE, y, 60, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw state with appropriate color and label
		tft.setCursor(COL_VALUE, y);

		// BUTTON, SWITCH, or SELECTOR_MULTI
		if (state) {
			tft.setTextColor(TEST_COLOR_DIGITAL_ON);
			tft.print("[ ON ]");
		} else {
			tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
			tft.print("[OFF ]");
		}
	}

	static inline void updateJackDetector(uint8_t row, bool state) {
		uint8_t y = SOCKET_START_Y + (row * ROW_HEIGHT);

		// Clear state area
		tft.fillRect(COL_DETECTOR, y, 60, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw state
		tft.setCursor(COL_DETECTOR, y);

		if (state) {
			tft.setTextColor(TEST_COLOR_JACK_PRESENT);
			tft.print("[JACK]");
		} else {
			tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
			tft.print("[----]");
		}
	}

	static inline void updateJackReceiving(uint8_t row, bool receiving) {
		uint8_t y = SOCKET_START_Y + (row * ROW_HEIGHT);

		// Clear receiving area
		tft.fillRect(COL_VALUE + 60, y, 80, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw receiving state
		tft.setCursor(COL_VALUE + 60, y);

		if (receiving) {
			tft.setTextColor(TEST_COLOR_JACK_PRESENT);
			tft.print("[RX]");
		} else {
			tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
			tft.print("[--]");
		}
	}

	static inline void updateJackSending(uint8_t row, bool sending) {
		uint8_t y = SOCKET_START_Y + (row * ROW_HEIGHT);

		// Clear sending area
		tft.fillRect(COL_VALUE + 60, y, 80, ROW_HEIGHT - 2, TEST_COLOR_BACKGROUND);

		// Draw sending state
		tft.setCursor(COL_VALUE + 60, y);

		if (sending) {
			tft.setTextColor(TEST_COLOR_JACK_PRESENT);
			tft.print("[TX]");
		} else {
			tft.setTextColor(TEST_COLOR_DIGITAL_OFF);
			tft.print("[--]");
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

	// Multi-module mode display methods
	static inline void clearScreen() {
		tft.fillScreen(TEST_COLOR_BACKGROUND);
	}

	// Set layout for multi-module mode (tab bar + header + info takes 52px)
	static inline void setMultiModuleLayout() {
		ANALOG_HEADER_Y = 52;
		ANALOG_START_Y = 68;
	}

	// Reset to single-module layout
	static inline void setSingleModuleLayout() {
		ANALOG_HEADER_Y = 36;
		ANALOG_START_Y = 52;
	}

	static inline void drawTabBar(uint8_t currentIndex, uint8_t totalModules) {
		static constexpr uint8_t TAB_HEIGHT = 20;
		static constexpr uint16_t TAB_ACTIVE_COLOR = 0x07FF;    // Cyan
		static constexpr uint16_t TAB_INACTIVE_COLOR = 0x4208;  // Dark gray

		uint16_t tabWidth = 320 / totalModules;

		for (uint8_t i = 0; i < totalModules; ++i) {
			uint16_t x = i * tabWidth;
			uint16_t color = (i == currentIndex) ? TAB_ACTIVE_COLOR : TAB_INACTIVE_COLOR;

			// Draw tab background
			tft.fillRect(x, 0, tabWidth - 1, TAB_HEIGHT, color);

			// Draw tab number
			tft.setTextColor(ILI9341_BLACK);
			tft.setTextSize(1);
			// Center the number in the tab
			uint8_t textX = x + (tabWidth / 2) - 3;
			tft.setCursor(textX, 6);
			tft.print(i + 1);
		}
	}

	static inline void drawMultiModuleHeader(const char* moduleName, uint8_t slot,
	                                          uint8_t typeId, uint8_t moduleNum, uint8_t totalModules) {
		static constexpr uint8_t MULTI_HEADER_Y = 20;
		static constexpr uint8_t MULTI_INFO_Y = 36;

		// Header line
		tft.setTextColor(TEST_COLOR_HEADER);
		tft.setCursor(COL_NAME, MULTI_HEADER_Y);
		tft.print("Module ");
		tft.print(moduleNum);
		tft.print('/');
		tft.print(totalModules);
		tft.print(" - ");
		tft.println(moduleName);

		// Info line
		tft.setTextColor(TEST_COLOR_LABEL);
		tft.setCursor(COL_NAME, MULTI_INFO_Y);
		tft.print("Slot: ");
		tft.print(slot);
		tft.print("   Type: ");
		// Print all 8 binary digits with leading zeros
		for (int8_t i = 7; i >= 0; --i) {
			tft.print((typeId >> i) & 1);
		}
	}

};

#undef tft
