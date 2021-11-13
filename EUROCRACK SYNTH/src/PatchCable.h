#ifndef __PATCH_CABLE_H__
#define __PATCH_CABLE_H__

#include <list>
#include <memory>
#include <Audio.h>

#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "HardwareCfg.h"

typedef  std::shared_ptr<InputSocket> is_ptr;
typedef  std::shared_ptr<OutputSocket> os_ptr;

class PatchCable {
	private:
		AudioConnection* ac[POLYPHONY];
		uint_fast8_t connectionType;
		static std::list <std::unique_ptr<PatchCable>> activeConnections;
		static void searchForCablesToAdd();
	public:
		//ctor
		PatchCable(os_ptr, is_ptr);
		
		//dtor
		~PatchCable();
		
		unsigned int inputSocket_uid;
		unsigned int outputSocket_uid;

		static bool checkConnection (os_ptr, is_ptr);
		
		static void addFromInput(is_ptr);
		static void addFromOutput(os_ptr);
		static void deleteFromInput(is_ptr);
		static void deleteFromOutput(os_ptr);

		//bool operator==(std::shared_ptr<PatchCable> other) const {return this->_in==other->getInputSocket()&&this->_out==other->getOutputSocket();};
};

#endif