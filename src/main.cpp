/*
 Name:		EUROCRACK_SYNTH.ino
 Created:	10/10/2021 11:05:07 PM
 Author:	cristian
*/

//#include libraries

#include <Arduino.h>

#include <vector>
#include <Audio.h>
#include <SD.h>
#include <SdFat.h>
#include <ILI9341_t3.h>
#include <SPI.h>
#include <Wire.h>

//include my classes
#include "core/module.h"
#include "core/address.h"
#include "core/patch_cable.h"
#include "configuration.h"
#include "hardware_setup/motherboard.h"
#include "services/synth_display.h"
#include "services/synth_touch.h"
#include "services/module_manager.h"
#include "services/patch_cable_manager.h"

#if TEST_MODE_ENABLED
#include "test/test_mode.h"
#endif

//AudioControlSGTL5000     sgtl5000_1;

//global variables
USBHost usbHost;
MIDIDevice midiOnUsbHost(usbHost);

#define POLYPHONIC

void setup() {
	// Initialize all motherboard pins (mux selectors, touchscreen, display)
	Motherboard::init();

	#if TEST_MODE_ENABLED
	// Test mode needs much less audio memory
	AudioMemory(200);
	#else
	// Production mode needs more for full synth operation
	AudioMemory(1500);
	#endif

	delay(2000);

	SynthDisplay::init();
	SynthTouch::init();

	#if TEST_MODE_ENABLED
	// Enter multi-module test mode with touchscreen support
	pinMode(pins::READ, INPUT_PULLDOWN);
	TestMode::enterMultiModule();
	pinMode(pins::READ, INPUT);
	return;
	#endif

	#if CONFIGURATION__USBHOST_ENABLED
	usbHost.begin();
	#endif

	#if CONFIGURATION__SERIAL_ENABLED
	Serial.begin(31250);
	#endif

	#if CONFIGURATION__SDCARD_ENABLED
	SD.begin(BUILTIN_SDCARD);
	#endif

	pinMode(pins::READ, INPUT_PULLDOWN);

	ModuleManager::factory();

	pinMode(pins::READ, INPUT);

	// Reset all multiplexer selectors
	Motherboard::resetMuxSelectors();
}

void loop() {
	#if TEST_MODE_ENABLED
	SynthTouch::update();
	TestMode::updateMultiModule();
	return;
	#endif

	ModuleManager::updateAll();
	PatchCableManager::update();
}

