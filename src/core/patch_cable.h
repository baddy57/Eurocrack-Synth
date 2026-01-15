#pragma once

#include <memory>
#include <Audio.h>

#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "hardware_cfg.h"

#define POLYPHONY 4

class PatchCable {
	private:
		AudioConnection* ac[POLYPHONY];
		uint_fast8_t connectionType;

	public:
		PatchCable(std::shared_ptr<OutputSocket>, std::shared_ptr<InputSocket>);
		~PatchCable();

		std::shared_ptr<InputSocket> inputSocket;
		std::shared_ptr<OutputSocket> outputSocket;

		static bool checkConnection(std::shared_ptr<OutputSocket>, std::shared_ptr<InputSocket>);
};
