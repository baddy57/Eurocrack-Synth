#pragma once

#include <cstdint>

#include <ILI9341_t3.h>
//#include <font_Arial.h> // from ILI9341_t3
//#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <USBHost_t36.h>

#include "../configuration.h"

#if CONFIGURATION__MOBO_REV_3
#include "motherboard_pins_rev3.h"

#elif CONFIGURATION__MOBO_REV_4
#include "motherboard_pins_rev4.h"

#elif CONFIGURATION__MOBO_REV_5
#include "motherboard_pins_rev5.h"

#endif

const bool ID_MUX_LOCATION [3] = {0, 0, 0}; //IN_SEL2[0, 1, 2] --->pins::RG, pins::RH, pins::RI

const uint_fast8_t OUT_SEL[6]	= 	{pins::WC, pins::WB, pins::WA, pins::WF, pins::WE, pins::WD};	//OUT SEL
const uint_fast8_t OUT_SEL2[3]	= 	{pins::WI, pins::WH, pins::WG};									//OUT SEL2

const uint_fast8_t IN_SEL[6]	= 	{pins::RC, pins::RB, pins::RA, pins::RF, pins::RE, pins::RD};	//IN SEL
const uint_fast8_t IN_SEL2[6] 	=	{pins::RG, pins::RH, pins::RI, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for small module (4053)

//const uint_fast8_t IN_SEL2[6] 	=	{pins::RI, pins::RH, pins::RG, pins::RL, pins::RK, pins::RJ};	//IN SEL2
//ok for big module (4051)

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







//USBHost usbHost;

//XPT2046_Touchscreen ts(pins::TOUCH_CS, pins::TOUCH_IRQ);
//XPT2046_Touchscreen ts(pins::TOUCH_CS); // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(pins::TOUCH_CS, 255); // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(pins::TOUCH_CS, pins::TOUCH_IRQ); // Param 2 - Touch IRQ Pin - interrupt enabled polling


