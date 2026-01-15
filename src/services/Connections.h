#pragma once

#include <vector>
#include <list>
#include <memory>

class InputSocket;
class OutputSocket;
class PatchCable;

class PatchCableManager {
	// Private: cable management
	static std::list<std::unique_ptr<PatchCable>> _cables;
	static void searchForCablesToAdd();

public:
	// All registered sockets
	static std::vector<std::shared_ptr<InputSocket>> inputs;
	static std::vector<std::shared_ptr<OutputSocket>> outputs;

	// State buckets for connection matching
	static std::list<std::shared_ptr<InputSocket>> availableInputs;
	static std::list<std::shared_ptr<InputSocket>> busyInputs;
	static std::list<std::shared_ptr<OutputSocket>> availableOutputs;

	// Socket registration (called by modules at construction)
	static void registerInput(std::shared_ptr<InputSocket> socket);
	static void registerOutput(std::shared_ptr<OutputSocket> socket);

	// Jack events (called when physical jack state changes)
	static void onInputConnected(std::shared_ptr<InputSocket> socket);
	static void onInputDisconnected(std::shared_ptr<InputSocket> socket);
	static void onOutputConnected(std::shared_ptr<OutputSocket> socket);
	static void onOutputDisconnected(std::shared_ptr<OutputSocket> socket);

	// State transitions
	static void setInputAvailable(std::shared_ptr<InputSocket> socket);
	static void setInputBusy(std::shared_ptr<InputSocket> socket);
	static void setInputInactive(std::shared_ptr<InputSocket> socket);
	static void setOutputAvailable(std::shared_ptr<OutputSocket> socket);
	static void setOutputInactive(std::shared_ptr<OutputSocket> socket);

	// Main loop update - checks all jack states
	static void update();
};
