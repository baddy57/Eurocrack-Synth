#ifndef __HARDWARE_CFG_H__
#define __HARDWARE_CFG_H__ 

#include <cstdint>

#include <ILI9341_t3.h>
//#include <font_Arial.h> // from ILI9341_t3
//#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <USBHost_t36.h>

//PINS
namespace pins {
	enum {
		RX, 
		TX, 
		WD, 
		WE, 
		WF, 
		LCD_DC, 
		WG, 
		T_CS=9, 
		WH, 
		__MOSI, 
		__MISO, 
		__SCK, 
		READ, 
		VOLUME, 
		__IRQ=16, 
		WC, 
		WRITE=22, 
		WA=24, 
		RGB1_3, 
		WB, 
		RGB0_3, 
		WI, 
		RL, 
		RK, 
		RJ, 
		RI, 
		RA, 
		RB, 
		RC, 
		LCD_CS, 
		RH, 
		RG, 
		RF, 
		RE, 
		RD};
};

const uint_fast8_t MIDI_OUT_PIN = 	pins::TX;
const uint_fast8_t MIDI_IN_PIN 	= 	pins::RX;


const uint_fast8_t IN_SEL[6]	= 	{pins::RC, pins::RB, pins::RA, pins::RF, pins::RE, pins::RD};	//IN SEL
const uint_fast8_t IN_SEL2[6] 	=	{pins::RG, pins::RH, pins::RI, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for small module (4053)

//const uint_fast8_t IN_SEL2[6] 	=	{pins::RI, pins::RH, pins::RG, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for big module (4051)

const uint_fast8_t OUT_SEL[6]	= 	{pins::WC, pins::WB, pins::WA, pins::WF, pins::WE, pins::WD};	//OUT SEL
const uint_fast8_t OUT_SEL2[3]	= 	{pins::WI, pins::WH, pins::WG};									//OUT SEL2



const uint_fast8_t READ_PIN 	=	pins::READ;  
const uint_fast8_t WRITE_PIN 	=	pins::WRITE;
const uint_fast8_t VOLUME_PIN 	=	pins::VOLUME;

const bool ID_MUX_LOCATION [3] = {0, 0, 0}; //IN_SEL2[0, 1, 2] --->pins::RG, pins::RH, pins::RI



#define CS_PIN pins::T_CS //14
#define TFT_DC pins::LCD_DC	//5
#define TFT_CS pins::LCD_CS	//36
// MOSI=11, MISO=12, SCK=13



#define MUX_DELAY 5
const float POT_DEADZONE = 5.f;
const uint_fast8_t POT_READS = 5;

const uint_fast8_t MAX_MODULES = 64;


const uint_fast8_t HIGH_UPDATE_PRIORITY = 255;
const uint_fast8_t MEDIUM_UPDATE_PRIORITY = 128;
const uint_fast8_t LOW_UPDATE_PRIORITY = 1;
const uint_fast8_t POLYPHONY = 10;


//USBHost myusb;

#define TIRQ_PIN 16//pins::__IRQS
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);
//XPT2046_Touchscreen ts(CS_PIN); // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255); // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN); // Param 2 - Touch IRQ Pin - interrupt enabled polling


//fix big module address
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
}*/


#endif