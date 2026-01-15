#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include <Audio.h>

#include "list"
#include "Address.h"
#include "Control.h"
#include "../controls/InputSocket.h"
#include "../controls/OutputSocket.h"
#include "../controls/Potentiometer.h"
#include "../controls/Button.h"
#include "../controls/Button2t.h"
#include "../controls/Switch.h"
#include "../controls/SelectorMulti.h"
#include "../controls/RotarySwitch.h"
#include "sw_components/GateIn.h"
#include "sw_components/Notes.h"
#include "sw_components/Scales.h"

#include "../services/SynthDisplay.h"
#include "ModuleInput.h"
#include "ModuleOutput.h"

#define tft SynthDisplay::raw()

class Module{
	private:

	protected:
		Address moduleAddress;
		std :: vector <AudioConnection*> internalConns;
		bool verbose;
	public:
		inline Module(const Address& address): moduleAddress(address), verbose(false){};

		virtual void updateValues()=0;
};