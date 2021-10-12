#include "Mixer8.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

//#include <vector>
#include <Audio.h>
#include <cstdint>

extern ILI9341_t3 tft;
namespace {
	enum _inputs {	GAIN7=8, GAIN6, GAIN5, GAIN4, GAIN3, GAIN2, GAIN1, GAIN0,//32-39
					CH0_D=16, CH1_D, CH2_D, CH3_D, CH4_D, CH5_D, CH6_D, CH7_D, //16-23
					CH7 =24, CH6, CH5, CH4, CH3, CH2, CH1, CH0, //48-53 fixed 202009061944
					OUT_STEREO_L_D=32, OUT_STEREO_R_D, SW, OUT_MONO_D //8-11
					};
	enum _outputs {OUT_MONO, OUT_STEREO_L, OUT_STEREO_R};
};

//ctor
Mixer8 :: Mixer8 (const Address& a)
	:	Module(a),
		_gain_pot0	(a, GAIN0),//32), 
		_gain_pot1	(a, GAIN1),//33), 
		_gain_pot2	(a, GAIN2),//34), 
		_gain_pot3	(a, GAIN3),//35), 
		_gain_pot4	(a, GAIN4),//36), 
		_gain_pot5	(a, GAIN5),//37), 
		_gain_pot6	(a, GAIN6),//38), 
		_gain_pot7	(a, GAIN7)//39), 
{
	// _channels0_3 = new AudioMixer4(); 
	// _channels4_7 = new AudioMixer4();
	// _mono_out    = new AudioMixer4();
	// _stereo_out_l= new AudioAmplifier();
	// _stereo_out_r= new AudioAmplifier();
	
	outputSocket.push_back(new OutputSocket(a, OUT_MONO, OUT_MONO_D, _mono_out, 0, "MIXR_OUT_MONO"));
	outputSocket.push_back(new OutputSocket(a, OUT_STEREO_L, OUT_STEREO_L_D, _stereo_out_l, 0,"MIXR_OUT_L"));
	outputSocket.push_back(new OutputSocket(a, OUT_STEREO_R, OUT_STEREO_R_D, _stereo_out_r, 0,"MIXR_OUT_R"));
	
	inputSocket.push_back(new InputSocket(a, CH0, CH0_D, _channels0_3, 0, "CH0"));
	inputSocket.push_back(new InputSocket(a, CH1, CH1_D, _channels0_3, 1, "CH1"));
	inputSocket.push_back(new InputSocket(a, CH2, CH2_D, _channels0_3, 2, "CH2"));
	inputSocket.push_back(new InputSocket(a, CH3, CH3_D, _channels0_3, 3, "CH3"));
	inputSocket.push_back(new InputSocket(a, CH4, CH4_D, _channels4_7, 0, "CH4"));
	inputSocket.push_back(new InputSocket(a, CH5, CH5_D, _channels4_7, 1, "CH5"));
	inputSocket.push_back(new InputSocket(a, CH6, CH6_D, _channels4_7, 2, "CH6"));
	inputSocket.push_back(new InputSocket(a, CH7, CH7_D, _channels4_7, 3, "CH7"));
	
	internalConns.push_back(new AudioConnection(_channels0_3, 0, _mono_out, 0));
	internalConns.push_back(new AudioConnection(_channels4_7, 0, _mono_out, 1));
	internalConns.push_back(new AudioConnection(_channels0_3, 0, _stereo_out_l, 0));
	internalConns.push_back(new AudioConnection(_channels4_7, 0, _stereo_out_r, 0));
	
	//verbose = true;
}

void
Mixer8 :: updateValues() {
	if(_gain_pot0.wasUpdated()){
		float g = _gain_pot0.f_read()-0.1;
		_channels0_3.gain(0, g);
		if(verbose){
			tft.print("f0 = ");
			tft.println(g);
		}
	}
	if(_gain_pot1.wasUpdated()){
		float g = _gain_pot1.f_read()-0.1;
		_channels0_3.gain(1, g);
		if(verbose){
			tft.print("f1 = ");
			tft.println(g);
		}
	}		
	if(_gain_pot2.wasUpdated()){
		float g = _gain_pot2.f_read()-0.1;
		_channels0_3.gain(2, g);
		if(verbose){
			tft.print("f2 = ");
			tft.println(g);
		}
	}
	if(_gain_pot3.wasUpdated()){
		float g = _gain_pot3.f_read()-0.1;
		_channels0_3.gain(3, g);
		if(verbose){
			tft.print("f3 = ");
			tft.println(g);
		}
	}
	if(_gain_pot4.wasUpdated()){
		float g = _gain_pot4.f_read()-0.1;
		_channels4_7.gain(4, g);
		if(verbose){
			tft.print("f4 = ");
			tft.println(g);
		}
	}
	if(_gain_pot5.wasUpdated()){
		float g = _gain_pot5.f_read()-0.1;
		_channels4_7.gain(5, g);
		if(verbose){
			tft.print("f5 = ");
			tft.println(g);
		}
	}
	if(_gain_pot6.wasUpdated()){
		float g = _gain_pot6.f_read()-0.1;
		_channels4_7.gain(6, g);
		if(verbose){
			tft.print("f6 = ");
			tft.println(g);
		}
	}
	if(_gain_pot7.wasUpdated()){
		float g = _gain_pot7.f_read()-0.1;
		_channels4_7.gain(7, g);
		if(verbose){
			tft.print("f7 = ");
			tft.println(g);
		}
	}
}
