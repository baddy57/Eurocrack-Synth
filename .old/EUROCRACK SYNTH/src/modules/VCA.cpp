#include "VCA.h"

extern ILI9341_t3 tft;

namespace {
	enum _outputs { OUT0, OUT1};
	enum _inputs {	OUT0_D,	IN0, IN0_D, CV0, CV0_D,
					IN1_D=27, IN1, CV1_D, CV1, OUT1_D			};
};

//ctor
VCA :: VCA (const Address& a)
	:	Module(a)
{
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT1, OUT1_D, _amp1, 0, "VCA_1_out"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN1, IN1_D, _amp1, 0, "VCA_1_in"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV1, CV1_D, _amp1, 1, "VCA_1_cv"));
}

void
VCA :: updateValues() {
	
}
