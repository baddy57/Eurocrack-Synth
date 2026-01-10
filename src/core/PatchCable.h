#pragma once

#include <list>
#include <memory>
#include <Audio.h>

#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "HardwareCfg.h"

#define POLYPHONY 4

class PatchCable {
	private:
		AudioConnection* ac[POLYPHONY];
		
		uint_fast8_t connectionType;
		
		static std::list <std::unique_ptr<PatchCable>> activeCables;

		static void searchForCablesToAdd();
	public:
		//ctor
		PatchCable(std::shared_ptr<OutputSocket>, std::shared_ptr<InputSocket>);
		
		//dtor
		~PatchCable();
		
		std::shared_ptr<InputSocket> inputSocket;
		std::shared_ptr<OutputSocket> outputSocket;

		static bool checkConnection (std::shared_ptr<OutputSocket>, std::shared_ptr<InputSocket>);
		
		static void onInputSocketConnected(std::shared_ptr<InputSocket>);
		static void onOutputSocketConnected(std::shared_ptr<OutputSocket>);
		static void onInputSocketDisconnected(std::shared_ptr<InputSocket>);
		static void onOutputSocketDisconnected(std::shared_ptr<OutputSocket>);
};
