#include "Granular.h"

namespace {
	//in case of big module, use fix(pin) with inputs

	const uint8_t
	//inputs
		POT0(),
		POT1(),
		POT2(),
		SW0(),
		SW1(),
		SW2(),
		IN0(),
		IN1(),
		IN0_D(),
		IN1_D(),
		OUT0_D(),
	//outputs
		OUT0();


}
#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
	int16_t granularMemory[GRANULAR_MEMORY_SIZE];

Granular::Granular(const Address& a)
	:	Module(a)
	,	pot0	(a, POT0, PULLUP_VAL=0)
	...
{
	inputSockets.push_back(std::make_shared<InputSocket>(a, PIN, DET_PIN, AUDIOSTREAM, PORT, "NAME"));

	outputSockets.push_back(std::make_shared<OutputSocket>(a, PIN, DET_PIN, AUDIOSTREAM, PORT, "NAME"));

	internalConns.push_back(new AudioConnection(SRC_STREAM, PORT, DST_STREAM, PORT));

	...
	granular.begin
}

void Granular::updateValues() {
	if (pot0.wasUpdated()) {
		...
	}
	...
}


//add to modulefactory////////////////////////////
#include "modules/Granular.h"

case $IDd$: {
	Serial.print(moduleType);
	Serial.print("  Granular");
	Serial.print("  @  ");
	Serial.println(slotAddress.toInt());
	activeModules.push_back(new Granular(slotAddress));
	break;
}
/////////////////////////////////////////////////