/*
 Name:		EUROCRACK_SYNTH.ino
 Created:	10/10/2021 11:05:07 PM
 Author:	cristian
*/

//#include libraries

#include <vector>
#include <Audio.h>
#include <SD.h>
#include <SdFat.h>
#include <ILI9341_t3.h>
#include <SPI.h>
#include <Wire.h>

//include my classes
#include "src/Module.h"
#include "src/Address.h"
#include "src/PatchCable.h"
#include "src/HardwareCfg.h"
#include "src/ModuleFactory.h"

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

#define ADAFRUIT_NEOPIXEL_TEST 0
#if ADAFRUIT_NEOPIXEL_TEST
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(57, pins::RGB1_3, NEO_GRB + NEO_KHZ800);
#endif

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
	tft.fillScreen(ILI9341_BLUE);

	usbHost.begin();
	Serial.begin(31250);

	SD.begin(BUILTIN_SDCARD);

	module::factory(activeModules);


#if ADAFRUIT_NEOPIXEL_TEST
	strip.begin();
	strip.setBrightness(255);
	strip.show(); // Initialize all pixels to 'off'
#endif


}

void loop() {
	//update Modules
	for (auto i = activeModules.begin(),
		end = activeModules.end();
		i < end; ++i) {
		(*i)->updateValues();
	}
	Module::updateConnections();


#if ADAFRUIT_NEOPIXEL_TEST
	rainbow(5);
#endif

}


#if ADAFRUIT_NEOPIXEL_TEST
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
	for(i=0; i<strip.numPixels(); i++) {
	  strip.setPixelColor(i, Wheel((i+j) & 255));
	}
	strip.show();
	delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
	for(i=0; i< strip.numPixels(); i++) {
	  strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
	}
	strip.show();
	delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
	return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
	WheelPos -= 85;
	return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
#endif