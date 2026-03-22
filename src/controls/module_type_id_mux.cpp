#include "module_type_id_mux.h"

extern const uint_fast8_t IN_SEL2[];


ModuleTypeIdMux :: ModuleTypeIdMux(const Address& a){
//	tft.println("idmux ctor");
	a.setForReading();
	moduleId = 0;
	for (uint_fast8_t i = 0; i < 8; ++i) {
		digitalWrite(pins::RJ, bitRead(i, 0));
		digitalWrite(pins::RK, bitRead(i, 1));
		digitalWrite(pins::RL, bitRead(i, 2));
		delay(CONFIGURATION__MUX_DELAY_MICROSECONDS);
		bool digit(digitalRead(pins::READ));
		bitWrite(moduleId, i, digit);
	}
}
