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
#include "core/hardware_cfg.h"
#include "services/synth_display.h"
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

	pinMode(pins::RA, OUTPUT);
	pinMode(pins::RB, OUTPUT);
	pinMode(pins::RC, OUTPUT);
	pinMode(pins::RD, OUTPUT);
	pinMode(pins::RE, OUTPUT);
	pinMode(pins::RF, OUTPUT);
	pinMode(pins::RG, OUTPUT);
	pinMode(pins::RH, OUTPUT);
	pinMode(pins::RI, OUTPUT);
	pinMode(pins::RJ, OUTPUT);
	pinMode(pins::RK, OUTPUT);
	pinMode(pins::RL, OUTPUT);
	pinMode(pins::WA, OUTPUT);
	pinMode(pins::WB, OUTPUT);
	pinMode(pins::WC, OUTPUT);
	pinMode(pins::WD, OUTPUT);
	pinMode(pins::WE, OUTPUT);
	pinMode(pins::WF, OUTPUT);
	pinMode(pins::WG, OUTPUT);
	pinMode(pins::WH, OUTPUT);
	pinMode(pins::WI, OUTPUT);
	pinMode(pins::WRITE, OUTPUT);
	pinMode(pins::READ, INPUT);
	pinMode(pins::VOLUME, INPUT);
	// pinMode(pins::RGB1_3, OUTPUT);
	// pinMode(pins::RGB0_3, OUTPUT);

	AudioMemory(1500);

	delay(2000);

	SynthDisplay::init();

	#if TEST_MODE_ENABLED
	// Enter test mode and skip normal initialization
	pinMode(pins::READ, INPUT_PULLDOWN);
	TestMode::enter();
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

	//reset all mux selectors
	digitalWrite(pins::RA, LOW);
	digitalWrite(pins::RB, LOW);
	digitalWrite(pins::RC, LOW);
	digitalWrite(pins::RD, LOW);
	digitalWrite(pins::RE, LOW);
	digitalWrite(pins::RF, LOW);
	digitalWrite(pins::RG, LOW);
	digitalWrite(pins::RH, LOW);
	digitalWrite(pins::RI, LOW);
	digitalWrite(pins::RJ, LOW);
	digitalWrite(pins::RK, LOW);
	digitalWrite(pins::RL, LOW);
	digitalWrite(pins::WA, LOW);
	digitalWrite(pins::WB, LOW);
	digitalWrite(pins::WC, LOW);
	digitalWrite(pins::WD, LOW);
	digitalWrite(pins::WE, LOW);
	digitalWrite(pins::WF, LOW);
	digitalWrite(pins::WG, LOW);
	digitalWrite(pins::WH, LOW);
	digitalWrite(pins::WI, LOW);
}

void loop() {
	#if TEST_MODE_ENABLED
	TestMode::update();
	return;
	#endif

	ModuleManager::updateAll();
	PatchCableManager::update();
}

