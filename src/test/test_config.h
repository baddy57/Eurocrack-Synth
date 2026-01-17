#pragma once
#include <cstdint>
#include <vector>
#include <memory>

class Potentiometer;  // Forward declaration

#include "../controls/output_socket.h"
#include "../controls/input_socket.h"

enum class TestControlType : uint8_t {
	POTENTIOMETER,
	BUTTON,
	SWITCH,
	JACK_DETECTOR,
	SELECTOR_MULTI
};

// Struct for analog test control info - includes pointer to control for computed value
struct TestControlInfo {
	const char* name;
	uint8_t pinId;
	TestControlType type;
	Potentiometer* pot;  // Pointer to potentiometer for reading computed value (nullptr for digital)

	static TestControlInfo createAnalog(const char* n, uint8_t p, Potentiometer* ctrl) {
		return {n, p, TestControlType::POTENTIOMETER, ctrl};
	}

	static TestControlInfo createDigital(const char* n, uint8_t p, TestControlType t) {
		return {n, p, t, nullptr};
	}
};

// Struct for socket test info
struct TestSocketInfo {
	const char* name;
	bool isOutput;  // true = output socket, false = input socket
	std::shared_ptr<OutputSocket> outputSocket;
	std::shared_ptr<InputSocket> inputSocket;
	TestControlInfo detector;  // Jack detector control info

	static TestSocketInfo createOutput(std::shared_ptr<OutputSocket> socket) {
		return {socket->getName(), true, socket, nullptr, TestControlInfo::createDigital(socket->getName(), socket->jackDetector._modulePin, TestControlType::JACK_DETECTOR) };
	}

	static TestSocketInfo createInput(std::shared_ptr<InputSocket> socket) {
		return {socket->getName(), false, nullptr, socket, TestControlInfo::createDigital(socket->getName(), socket->jackDetector._modulePin, TestControlType::JACK_DETECTOR) };
	}
};
