#ifndef __PATCH_CABLE_H__
#define __PATCH_CABLE_H__

#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "HardwareCfg.h"

#include <Audio.h>
#include <list>
#include <memory>

class PatchCable {
	private:
		std::shared_ptr<InputSocket> _in;
		std::shared_ptr<OutputSocket> _out;
		AudioConnection* ac[POLYPHONY];
		uint_fast8_t connectionType;
		static std::list<std::shared_ptr<InputSocket>> inputsWithJack;
		static std::list<std::shared_ptr<OutputSocket>> outputsWithJack;
		static std::list <std::unique_ptr<PatchCable>> activeConnections;
		static void searchForCablesToAdd();
	public:
		//ctor
		PatchCable(std::shared_ptr<OutputSocket>, std::shared_ptr<InputSocket>);
		
		//dtor
		~PatchCable();
		
		std::shared_ptr<InputSocket> getInputSocket() const {return _in;};
		std::shared_ptr<OutputSocket> getOutputSocket() const {return _out;};

		static bool checkConnection (const std::shared_ptr<OutputSocket>, const std::shared_ptr<InputSocket>);
		
		static void addFromInput(std::shared_ptr<InputSocket>);
		static void addFromOutput(std::shared_ptr<OutputSocket>);
		static void deleteFromInput(std::shared_ptr<InputSocket>);
		static void deleteFromOutput(std::shared_ptr<OutputSocket>);

		bool operator==(std::shared_ptr<PatchCable> other) const {return this->_in==other->getInputSocket()&&this->_out==other->getOutputSocket();};
};

#endif