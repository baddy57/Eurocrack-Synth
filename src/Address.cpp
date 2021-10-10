#include "Address.h"
#include <Wire.h>
#include "HardwareCfg.h"

extern const uint_fast8_t IN_SEL[];
extern const uint_fast8_t IN_SEL2[];
extern const uint_fast8_t OUT_SEL[];
extern const uint_fast8_t OUT_SEL2[];
extern const uint_fast8_t READ_PIN;
extern const uint_fast8_t WRITE_PIN;
extern const bool ID_MUX_LOCATION[];
extern ILI9341_t3 tft;

//ctor
Address :: Address (uint_fast8_t id) {
	_id=id;
	//pin = READ_PIN;
	for (uint_fast8_t i=0; i<6; ++i)
		sel[5-i] = bitRead(id, i);
}

//copy ctor
Address :: Address (const Address& a){
	for (uint_fast8_t i=0; i<6; ++i)
		sel[i] = a.getSel(i);
	this->_id = a._id;					/////////2021.01.14
}

void
Address :: setForReading() const {
	for (uint_fast8_t i=0; i<6; ++i)
		digitalWrite(IN_SEL[i], sel[i]);
//	delayMicroseconds(MUX_DELAY);
	return;
}

//ctor
ControlAddress :: ControlAddress (const Address& slotAddress, uint_fast8_t id) : Address(slotAddress){
	pin = READ_PIN;
	for (uint_fast8_t i=0; i<6; ++i)
		sel2[5-i] = bitRead(id, i);
	this->_id = slotAddress._id + 64*id;					/////////2021.01.14
}
//ctor used for volume pot etc
ControlAddress :: ControlAddress (uint_fast8_t p) : Address(0){
	pin = p;
}

void
ControlAddress :: setForReading() const {
	Address :: setForReading();
	for (uint_fast8_t i=0; i<6; ++i)
		digitalWrite(IN_SEL2[i], sel2[i]);
    delayMicroseconds(MUX_DELAY);
}

//ctor
OutputSocketAddress :: OutputSocketAddress (const Address& slotAddress, uint_fast8_t id) : Address(slotAddress){
	pin = WRITE_PIN;  
	for (uint_fast8_t i=0; i<6; ++i)
		sel[i] = slotAddress.getSel(i);				////////////////202009052152 (5-) wrong cause getsel is already ok
	for (uint_fast8_t i=0; i<3; ++i)
		sel2[2-i] = bitRead(id, i);					////////////202009042209
	this->_id = slotAddress._id + 64*id;		/////////2021.01.14	
}

void
OutputSocketAddress :: setForWriting() const {
	for (uint_fast8_t i=0; i<6; ++i)
		digitalWrite(OUT_SEL[i], sel[i]);
	for (uint_fast8_t i=0; i<3; ++i)
		digitalWrite(OUT_SEL2[i], sel2[i]);
    delayMicroseconds(MUX_DELAY);
}

//ctor
IdMuxAddress :: IdMuxAddress(const Address& moduleAddress) : Address(moduleAddress){
	for(uint_fast8_t i=0; i<3; ++i){
		sel2[i] = ID_MUX_LOCATION[i]; //000
	}
}

void 
IdMuxAddress :: setForReading() const {
	Address :: setForReading();
	for (uint_fast8_t i=0; i<3; ++i)
		digitalWrite(IN_SEL2[2-i], sel2[i]);
    delayMicroseconds(MUX_DELAY);
}