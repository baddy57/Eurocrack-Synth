#include "WaveTable.h"

//PITCH BEND??
//SAVE TO SD

namespace {
	enum _inputs {	GATE0, GATE1, GATE2, GATE3,
					GATE0_D, GATE1_D, GATE2_D, GATE3_D,
					CV0, CV1, CV2, CV3,
					CV0_D, CV1_D, CV2_D, CV3_D,
					VEL0, VEL1, VEL2, VEL3,
					VEL0_D, VEL1_D, VEL2_D, VEL3_D,
					_B0, _B1, _B2, _B3,
					OUT_MONO_D	};
	enum _outputs {OUT_MONO};
};

//ctor
WaveTable :: WaveTable (const Address& a)
	:	Module(a),
		_b0	(a, _B0), 
		_b1	(a, _B1),
		_b2	(a, _B2),
		_b3	(a, _B3)/*,
		wt0(), 
		wt1(),
		wt2(),
		wt3()*/
	{
	
	outputSocket.push_back(std::make_shared<OutputSocket>(a, OUT_MONO, OUT_MONO_D, masterMixer, 0, "WT OUT"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, GATE0, GATE0_D, _gate0, 0, "WT GATE0"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, GATE1, GATE1_D, _gate1, 0, "WT GATE1"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, GATE2, GATE2_D, _gate2, 0, "WT GATE2"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, GATE3, GATE3_D, _gate3, 0, "WT GATE3"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV0, CV0_D, _cv0, 0, "WT CV0"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV1, CV1_D, _cv1, 0, "WT CV1"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV2, CV2_D, _cv2, 0, "WT CV2"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CV3, CV3_D, _cv3, 0, "WT CV3"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, VEL0, VEL0_D, _vel0, 0, "WT VEL0"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, VEL1, VEL1_D, _vel1, 0, "WT VEL1"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, VEL2, VEL2_D, _vel2, 0, "WT VEL2"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, VEL3, VEL3_D, _vel3, 0, "WT VEL3"));
	
	internalConns.push_back(new AudioConnection(wt0,0, masterMixer,0));
	internalConns.push_back(new AudioConnection(wt1,0, masterMixer,1));
	internalConns.push_back(new AudioConnection(wt2,0, masterMixer,2));
	internalConns.push_back(new AudioConnection(wt3,0, masterMixer,3));
}

void
WaveTable :: updateValues() {
	if(_b0.isPressed()){
		wt0.
	}

}