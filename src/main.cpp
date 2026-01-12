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
#include "core/Module.h"
#include "core/Address.h"
#include "core/PatchCable.h"
#include "core/HardwareCfg.h"
#include "core/ModuleFactory.h"

//constants
extern const uint_fast8_t HIGH_UPDATE_PRIORITY;
extern const uint_fast8_t MEDIUM_UPDATE_PRIORITY;
extern const uint_fast8_t LOW_UPDATE_PRIORITY;
extern const uint_fast8_t READ_PIN;
extern const uint_fast8_t WRITE_PIN;

//AudioControlSGTL5000     sgtl5000_1;

//global variables
std::vector<Module*> activeModules;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

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
	pinMode(pins::RGB1_3, OUTPUT);
	pinMode(pins::RGB0_3, OUTPUT);

	AudioMemory(1500);

	delay(2000);

	tft.begin();
	tft.setRotation(4);	
	tft.fillScreen(CONFIGURATION__BACKGROUND_COLOR);

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
	
	ModuleFactory::factory(activeModules);

	pinMode(pins::READ, INPUT);

}

void loop() {
	//update Modules
	for (auto i = activeModules.begin(),
		end = activeModules.end();
		i < end; ++i) {
		(*i)->updateValues();
	}
	Module::updateConnections();
}

