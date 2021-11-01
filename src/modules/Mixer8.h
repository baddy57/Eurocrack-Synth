#ifndef __MODULES_MIXER8_H__
#define __MODULES_MIXER8_H__ 1

#include "../Module.h"
#include "../Address.h"
#include <Audio.h>
#include <cstdint>
#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

class Mixer8 : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
		std :: vector <AudioConnection*> internalConns;//if any;
	*/
		//uint_fast8_t GAIN0, GAIN1, GAIN2, GAIN3, GAIN4, GAIN5, GAIN6, GAIN7;
		Potentiometer 	
			_gain_pot0, 
			_gain_pot1,
			_gain_pot2,
			_gain_pot3,
			_gain_pot4,
			_gain_pot5,
			_gain_pot6,
			_gain_pot7;
		
		AudioMixer4 
			_channels0_3, 
			_channels4_7,
			_mono_out;

		//AudioAmplifier 
			//_stereo_out_l, 
			//_stereo_out_r;
		
	public:
		Mixer8 () = delete;
		Mixer8 (const Address&);
		void updateValues(); //(uint_fast8_t priority) ?
		
};

#endif