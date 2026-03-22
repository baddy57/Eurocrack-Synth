#pragma once

#include <cstdint>

#include <ILI9341_t3.h>
//#include <font_Arial.h> // from ILI9341_t3
//#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <USBHost_t36.h>

#define MOBO_REV_3 0
#define MOBO_REV_4 0
#define MOBO_REV_5 1

#if MOBO_REV_3
//PINS
namespace pins {
	const byte RX = 0;
	const byte TX = 1;

	const byte WA = 24;
	const byte WB = 26;
	const byte WC = 17;
	const byte WD = 2;
	const byte WE = 3;
	const byte WF = 4;
	const byte WG = 6;
	const byte WH = 10;
	const byte WI = 28;

	const byte SPI_MOSI = 11;
	const byte SPI_MISO = 12;
	const byte SPI_SCK = 13;

	const byte LCD_CS = 36;
	const byte LCD_DC = 5;
	
	const byte TOUCH_CS = 9;
	const byte TOUCH_IRQ = 16;

	const byte READ = 14;
	const byte WRITE = 22;
	
	const byte RA = 33;
	const byte RB = 34;
	const byte RC = 35;
	const byte RD = 41;
	const byte RE = 40;
	const byte RF = 39;
	const byte RG = 38;
	const byte RH = 37;
	const byte RI = 32;
	const byte RJ = 31;
	const byte RK = 30;
	const byte RL = 29;
	
	const byte VOLUME = 15;
	
	const byte RGB1_3 = 25;
	const byte RGB0_3 = 27;
};

#elif MOBO_REV_4
namespace pins {
	const byte WA = 27;
	const byte WB = 26;
	const byte WC = 17;
	const byte WD = 2;
	const byte WE = 3;
	const byte WF = 4;
	const byte WG = 6;
	const byte WH = 10;
	const byte WI = 28;

	const byte SPI_MOSI = 11;
	const byte SPI_MISO = 12;
	const byte SPI_SCK = 13;

	const byte LCD_CS = 36;
	const byte LCD_DC = 5;

	const byte TOUCH_CS = 9;
	const byte TOUCH_IRQ = 16;

	const byte READ = 14;
	const byte WRITE = 22;

	const byte TX = 24;
	const byte RX = 25;

	const byte RA = 33;
	const byte RB = 34;
	const byte RC = 35;
	const byte RD = 41;
	const byte RE = 40;
	const byte RF = 39;
	const byte RG = 38;
	const byte RH = 37;
	const byte RI = 32;
	const byte RJ = 31;
	const byte RK = 30;
	const byte RL = 29;

	const byte VOLUME = 15;
};
#elif MOBO_REV_5
namespace pins {
	const byte SERIAL_USB_RX = 0;
	const byte SERIAL_USB_TX = 1;
	
	const byte WA = 27;
	const byte WB = 26;
	const byte WC = 17;
	const byte WD = 2;
	const byte WE = 3;
	const byte WF = 4;
	const byte WG = 6;
	const byte WH = 10;
	
	const byte SPI_MOSI = 11;
	const byte SPI_MISO = 12;
	const byte SPI_SCK = 13;
	
	const byte LCD_CS = 36;
	const byte LCD_DC = 37;
	
	const byte TOUCH_CS = 9;
	const byte TOUCH_IRQ = 16;
	
	const byte READ = 14;
	const byte WRITE = 22;
	
	const byte TX = 24;
	const byte RX = 25;

	const byte WI = 28;
	const byte RL = 29;
	const byte RA = 33;
	const byte RB = 34;
	const byte RC = 35;
	const byte RD = 41;
	const byte RE = 40;
	const byte RF = 39;
	const byte RG = 38;
	const byte RH = 5;
	const byte RI = 32;
	const byte RJ = 31;
	const byte RK = 30;
	
	const byte VOLUME = 15;// unused
#endif
};

const uint_fast8_t IN_SEL[6]	= 	{pins::RC, pins::RB, pins::RA, pins::RF, pins::RE, pins::RD};	//IN SEL
const uint_fast8_t IN_SEL2[6] 	=	{pins::RG, pins::RH, pins::RI, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for small module (4053)

//const uint_fast8_t IN_SEL2[6] 	=	{pins::RI, pins::RH, pins::RG, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for big module (4051)

const uint_fast8_t OUT_SEL[6]	= 	{pins::WC, pins::WB, pins::WA, pins::WF, pins::WE, pins::WD};	//OUT SEL
const uint_fast8_t OUT_SEL2[3]	= 	{pins::WI, pins::WH, pins::WG};									//OUT SEL2


const bool ID_MUX_LOCATION [3] = {0, 0, 0}; //IN_SEL2[0, 1, 2] --->pins::RG, pins::RH, pins::RI

#define CONFIGURATION__MUX_DELAY_MICROSECONDS 5
const float CONFIGURATION__POT_DEADZONE = 5.f;
const uint_fast8_t CONFIGURATION__POT_READS = 5;

const uint_fast8_t CONFIGURATION__MAX_MODULES = 16;

//USBHost usbHost;

//XPT2046_Touchscreen ts(pins::TOUCH_CS, pins::TOUCH_IRQ);
//XPT2046_Touchscreen ts(pins::TOUCH_CS); // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(pins::TOUCH_CS, 255); // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(pins::TOUCH_CS, pins::TOUCH_IRQ); // Param 2 - Touch IRQ Pin - interrupt enabled polling


//fix big module address
//#warning "Obsolete, should be fixed via hardware"
#define fix(x)                  \
    (x)>32 && (x)%16>=8 ?       \
        (x) :                   \
    ((x>=32) ?                  \
        (x-24) :                \
    ((x%16)>=8 ?                \
        (x+24) :                \
        (x)))
/*unsigned int Module::fix(const unsigned int i)
{
	if (i >= 32 && i % 16 >= 8)
		return i;
	if (i >= 32)
		return i - 24;
	if (i % 16 >= 8)
		return i + 24;
	return i;

	BANK A (8-15) ←→ BANK D (32-39)
	BANK C (24-31) ←→ BANK F (48-55)
	BANK B (16-23) OK
	BANK E (40-47) OK
	BANK G (56-63) OK
}*/


#define CONFIGURATION__LOGGER__JACK_EVENTS 1
#define CONFIGURATION__LOGGER__CONNECTIONS 1
#define CONFIGURATION__LOGGER__MODULE_FACTORY 1
#define CONFIGURATION__LOGGER__ANALOG_RAW 1
#define CONFIGURATION__LOGGER__ANALOG_SMOOTHED 1
#define CONFIGURATION__LOGGER__CONTROL_CHANGES 1

#define CONFIGURATION__BACKGROUND_COLOR ILI9341_BLACK

#define CONFIGURATION__SERIAL_ENABLED 0
#define CONFIGURATION__SDCARD_ENABLED 0
#define CONFIGURATION__USBHOST_ENABLED 0

#define TEST_MODE_ENABLED 1