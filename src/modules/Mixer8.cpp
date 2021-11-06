#include "Mixer8.h"

extern ILI9341_t3 tft;

namespace {
	enum FIXED_inputs {	//GAIN7=8, GAIN6, GAIN5, GAIN4, GAIN3, GAIN2, GAIN1, GAIN0,//32-39
					CH0_D=16, CH1_D, CH2_D, CH3_D, CH4_D, CH5_D, CH6_D, CH7_D, //16-23
					CH7 =24, CH6, CH5, CH4, CH3, CH2, CH1, CH0, //48-53 fixed 202009061944
					OUT_STEREO_L_D=32, OUT_STEREO_R_D, SW, OUT_MONO_D //8-11
					};
	const uint_fast8_t
		GAIN0(fix(39)),
		GAIN1(fix(38)),
		GAIN2(fix(37)),
		GAIN3(fix(36)),
		GAIN4(fix(35)),
		GAIN5(fix(34)),
		GAIN6(fix(33)),
		GAIN7(fix(32));
	enum _outputs {OUT_MONO, OUT_STEREO_L, OUT_STEREO_R};
};

//ctor
Mixer8 :: Mixer8 (const Address& a)
	:	Module(a)
	,	_gain_pot0	(a, GAIN0, 4700.f)
	,	_gain_pot1	(a, GAIN1, 4700.f)
	,	_gain_pot2	(a, GAIN2, 4700.f)
	,	_gain_pot3	(a, GAIN3, 4700.f)
	,	_gain_pot4	(a, GAIN4, 4700.f)
	,	_gain_pot5	(a, GAIN5, 4700.f)
	,	_gain_pot6	(a, GAIN6, 4700.f)
	,	_gain_pot7	(a, GAIN7, 4700.f)
{

	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_MONO, OUT_MONO_D, _mono_out, 0, "MIXR_OUT_MONO"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_STEREO_L, OUT_STEREO_L_D, _channels0_3, 0,"MIXR_OUT_L"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT_STEREO_R, OUT_STEREO_R_D, _channels4_7, 0,"MIXR_OUT_R"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH0, CH0_D, _channels0_3, 0, "CH0"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH1, CH1_D, _channels0_3, 1, "CH1"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH2, CH2_D, _channels0_3, 2, "CH2"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH3, CH3_D, _channels0_3, 3, "CH3"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH4, CH4_D, _channels4_7, 0, "CH4"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH5, CH5_D, _channels4_7, 1, "CH5"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH6, CH6_D, _channels4_7, 2, "CH6"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, CH7, CH7_D, _channels4_7, 3, "CH7"));
	
	internalConns.push_back(new AudioConnection(_channels0_3, 0, _mono_out, 0));
	internalConns.push_back(new AudioConnection(_channels4_7, 0, _mono_out, 1));

	_channels0_3.gain(0, 0);
	_channels0_3.gain(1, 0);
	_channels0_3.gain(2, 0);
	_channels0_3.gain(3, 0);
	_channels4_7.gain(0, 0);
	_channels4_7.gain(1, 0);
	_channels4_7.gain(2, 0);
	_channels4_7.gain(3, 0);
}

void
Mixer8 :: updateValues() {
	if(_gain_pot0.wasUpdated()){
		float g = _gain_pot0.f_read();

		_channels0_3.gain(0, g);
	}
	if(_gain_pot1.wasUpdated()){
		float g = _gain_pot1.f_read();

		_channels0_3.gain(1, g);

	}		
	if(_gain_pot2.wasUpdated()){
		float g = _gain_pot2.f_read();
		_channels0_3.gain(2, g);

	}
	if(_gain_pot3.wasUpdated()){
		float g = _gain_pot3.f_read();
		_channels0_3.gain(3, g);

	}
	if(_gain_pot4.wasUpdated()){
		float g = _gain_pot4.f_read();
		_channels4_7.gain(4, g);

	}
	if(_gain_pot5.wasUpdated()){
		float g = _gain_pot5.f_read();
		_channels4_7.gain(5, g);
	}
	if(_gain_pot6.wasUpdated()){
		float g = _gain_pot6.f_read();
		_channels4_7.gain(6, g);
	}
	if(_gain_pot7.wasUpdated()){
		float g = _gain_pot7.f_read();
		_channels4_7.gain(7, g);
	}
}
