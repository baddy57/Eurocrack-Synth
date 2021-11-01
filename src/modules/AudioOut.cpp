#include "AudioOut.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

extern ILI9341_t3 tft;

//#include <vector>
#include <Audio.h>
#include <cstdint>

extern AudioControlSGTL5000 _audioCtrl;

namespace {
	enum _inputs {	ANALOG_OUT_L_D=16, ANALOG_OUT_L, 
					ANALOG_OUT_R_D, ANALOG_OUT_R, 
					USB_OUT_L_D, USB_OUT_L, 
					USB_OUT_R_D, USB_OUT_R=23
					};
};
AudioControlSGTL5000 AudioOut::_audioCtrl;
//CONSTRUCTOR
AudioOut :: AudioOut(const Address& a) : 
		Module(a), 
		//_audioCtrl(),
		//_analogOut(),
		//_usbOut(),
		_volume_pot0(pins::VOLUME)
{

	_audioCtrl.enable();
	_audioCtrl.adcHighPassFilterDisable(); //http://prajtm.scripts.mit.edu/projects/portfolio/portable-synth-v1-prototype/
	_audioCtrl.lineInLevel(2);
										   //bool x = _audioCtrl.inputSelect(AUDIO_INPUT_MIC);

	_audioCtrl.volume(0.5);

	inputSockets.push_back(std::make_shared<InputSocket> (a, ANALOG_OUT_L, ANALOG_OUT_L_D, _analogOut, 0, "ANALOG_OUT_L"));
	inputSockets.push_back(std::make_shared<InputSocket> (a, ANALOG_OUT_R, ANALOG_OUT_R_D, _analogOut, 1, "ANALOG_OUT_R"));
	inputSockets.push_back(std::make_shared<InputSocket> (a, USB_OUT_L, USB_OUT_L_D, _usbOut, 0, "USB_OUT_L"));
	inputSockets.push_back(std::make_shared<InputSocket> (a, USB_OUT_R, USB_OUT_R_D, _usbOut, 1, "USB_OUT_R"));
	
}
//#define DIV1023 1/1023
void
AudioOut :: updateValues() {
	if(_volume_pot0.wasUpdated()){
		_audioCtrl.volume(_volume_pot0.f_read());
	}
	return;
}