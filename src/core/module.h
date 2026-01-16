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
#include "features/gate_in.h"
#include "features/notes.h"
#include "features/scales.h"

#include "../services/synth_display.h"
#include "module_input.h"
#include "module_output.h"
#include "../test/test_config.h"
#include "module_type_ids.h"


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

		virtual uint8_t getModuleTypeId() const { return 0; }
		
		virtual const char* getModuleName() const { return "Unknown"; }
		
		#if TEST_MODE_ENABLED
		// Test mode support - modules override to provide their control info
		virtual void getTestControls(
			std::vector<TestControlInfo>& analog,
			std::vector<TestControlInfo>& digital) = 0;

		// Test mode support - modules override to provide their socket info
		virtual void getTestSockets(std::vector<TestSocketInfo>& sockets) {}
		#endif

		const Address& getAddress() const { return moduleAddress; }
};