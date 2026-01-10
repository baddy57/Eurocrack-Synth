#pragma once

#include <list>
#include <memory>
#include <Audio.h>

#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "HardwareCfg.h"

class PatchCable {
	private:
		AudioConnection* ac[POLYPHONY];
		
		uint_fast8_t connectionType;
		
		static std::list <std::unique_ptr<PatchCable>> activeCables;

		static void searchForCablesToAdd();
	public:
		//ctor
		PatchCable(OutputSocket_p, InputSocket_p);
		
		//dtor
		~PatchCable();
		
		InputSocket_p inputSocket;
		OutputSocket_p outputSocket;

		static bool checkConnection (OutputSocket_p, InputSocket_p);
		
		static void onInputSocketConnected(InputSocket_p);
		static void onOutputSocketConnected(OutputSocket_p);
		static void onInputSocketDisconnected(InputSocket_p);
		static void onOutputSocketDisconnected(OutputSocket_p);
};
