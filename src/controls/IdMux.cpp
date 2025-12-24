#include "IdMux.h"

extern const uint_fast8_t IN_SEL2[];
extern const uint_fast8_t READ_PIN;
extern ILI9341_t3 tft;


//idmux ctor
IdMux :: IdMux(const Address& a){
//	tft.println("idmux ctor");
	a.setForReading();
	moduleId = 0;
	for (uint_fast8_t i = 0; i < 8; ++i) {
		digitalWrite(pins::RJ, bitRead(i, 0));
		digitalWrite(pins::RK, bitRead(i, 1));
		digitalWrite(pins::RL, bitRead(i, 2));
		delay(MUX_DELAY);
		bool digit(digitalRead(READ_PIN));
		bitWrite(moduleId, i, digit);
	}
}
