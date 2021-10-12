#ifndef __MODULES_AUDIO_IN_H__
#define __MODULES_AUDIO_IN_H__

#include "../Module.h"
#include "../Address.h"

class AudioIn : public Module {
	private:
	/*inherited
		Address moduleAddress;
		std :: vector <Socket> inputSocket; //[8];
		std :: vector <Socket> outputSocket;//[8];
	*/
		AudioInputI2S _lineIn;
		AudioInputUSB _usbIn;
	//	Potentiometer _micGain;
	public:
		AudioIn(const Address&);		
		void updateValues() override; 
		void updateConnections();
};

#endif