#pragma once

#include "../core/Module.h"

namespace VCA_pins {
	enum outputs { OUT0, OUT1 };
	enum inputs { OUT0_D, IN0, IN0_D, CV0, CV0_D,
				  IN1_D=27, IN1, CV1_D, CV1, OUT1_D };
}

class VCA : public Module {
private:
	AudioEffectMultiply _amp0, _amp1;

public:
	inline VCA(const Address& a) : Module(a) {
		using namespace VCA_pins;
		outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT1, OUT1_D, _amp1, 0, "VCA_1_out"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, IN1, IN1_D, _amp1, 0, "VCA_1_in"));
		inputSockets.push_back(std::make_shared<InputSocket>(a, CV1, CV1_D, _amp1, 1, "VCA_1_cv"));
	}

	inline void updateValues() override {
	}
};
