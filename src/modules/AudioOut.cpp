#include "AudioOut.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

extern ILI9341_t3 tft;

//#include <vector>
#include <Audio.h>
#include <cstdint>

namespace {
	enum _inputs {	ANALOG_OUT_L_D=16, ANALOG_OUT_L, 
					ANALOG_OUT_R_D, ANALOG_OUT_R, 
					USB_OUT_L_D, USB_OUT_L, 
					USB_OUT_R_D, USB_OUT_R=23
					};
};

//CONSTRUCTOR
AudioOut :: AudioOut(const Address& a) : 
		Module(a), 
		_audioCtrl(),
		_volume_pot0(pins::VOLUME),
		_analogOut(),
		_usbOut()
{
	_audioCtrl.enable();
	_audioCtrl.volume(0.2);
	// _audioCtrl.adcHighPassFilterDisable(); //http://prajtm.scripts.mit.edu/projects/portfolio/portable-synth-v1-prototype/

	inputSocket.push_back(new InputSocket (a, ANALOG_OUT_L, ANALOG_OUT_L_D, _analogOut, 0, "ANALOG_OUT_L"));
	inputSocket.push_back(new InputSocket (a, ANALOG_OUT_R, ANALOG_OUT_R_D, _analogOut, 1, "ANALOG_OUT_R"));
	inputSocket.push_back(new InputSocket (a, USB_OUT_L, USB_OUT_L_D, _usbOut, 0, "USB_OUT_L"));
	inputSocket.push_back(new InputSocket (a, USB_OUT_R, USB_OUT_R_D, _usbOut, 1, "USB_OUT_R"));
	
}
//#define DIV1023 1/1023
void
AudioOut :: updateValues() {
	if(_volume_pot0.wasUpdated()){
		_audioCtrl.volume(_volume_pot0.f_read());
	}
	return;
}

// void
// AudioOut :: updateConnections() {
	// tft.println("AudioOut :: updateConnections()");
	// Module :: updateConnections();
// }
