#include "VCA.h"

#include "Module.h"
#include "Address.h"
/* 
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"
 */
#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <vector>
#include <Audio.h>
#include <cstdint>
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
	
//	outputSocket.push_back(new OutputSocket(a, OUT0, OUT0_D, _amp0, 0, "VCA_0_out"));
	outputSocket.push_back(new OutputSocket(a, OUT1, OUT1_D, _amp1, 0, "VCA_1_out"));
	
//	inputSocket.push_back(new InputSocket(a, IN0, IN0_D, _amp0, 0, "VCA_0_in"));
//	inputSocket.push_back(new InputSocket(a, CV0, CV0_D, _amp0, 1, "VCA_0_cv"));
	inputSocket.push_back(new InputSocket(a, IN1, IN1_D, _amp1, 0, "VCA_1_in"));
	inputSocket.push_back(new InputSocket(a, CV1, CV1_D, _amp1, 1, "VCA_1_cv"));
	
}

void
VCA :: updateValues() {
	
}
