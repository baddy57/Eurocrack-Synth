#include "CONTROLS_IdMux.h"

#include "Control.h"
#include "HardwareCfg.h"
#include "Address.h"

#include <cstdint>

extern const uint_fast8_t IN_SEL2[];
extern const uint_fast8_t READ_PIN;
extern ILI9341_t3 tft;

//idmux ctor
IdMux :: IdMux(const Address& a) : address(a){
//	tft.println("idmux ctor");
}


uint_fast8_t
IdMux :: read() const {
//	tft.println("idmux::read");
	address.setForReading();	
	uint_fast8_t res = 0;
	for (uint_fast8_t i=0; i<8; ++i){
		digitalWrite(pins::RJ, bitRead(i, 0));
		digitalWrite(pins::RK, bitRead(i, 1));
		digitalWrite(pins::RL, bitRead(i, 2));
		delayMicroseconds(MUX_DELAY);
		bool x(digitalRead(READ_PIN));
		bitWrite(res, i, x);
	//	tft.print(x);
	}
//	tft.print(" = ");
//	tft.println(res);
	return res;
}