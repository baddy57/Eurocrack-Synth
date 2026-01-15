#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include <Audio.h>

#include "list"
#include "address.h"
#include "control.h"
#include "../controls/input_socket.h"
#include "../controls/output_socket.h"
#include "../controls/potentiometer.h"
#include "../controls/button.h"
#include "../controls/button2t.h"
#include "../controls/switch.h"
#include "../controls/selector_multi.h"
#include "../controls/rotary_switch.h"
#include "sw_components/gate_in.h"
#include "sw_components/notes.h"
#include "sw_components/scales.h"

#include "../services/synth_display.h"
#include "module_input.h"
#include "module_output.h"
#include "../test/test_config.h"

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

		// Test mode support - modules override to provide their control info
		virtual uint8_t getModuleTypeId() const { return 0; }
		virtual const char* getModuleName() const { return "Unknown"; }
		virtual void getTestControls(
			std::vector<TestControlInfo>& analog,
			std::vector<TestControlInfo>& digital) {}

		const Address& getAddress() const { return moduleAddress; }
};