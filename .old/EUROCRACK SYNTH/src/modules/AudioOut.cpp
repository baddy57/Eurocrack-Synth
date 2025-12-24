#include "AudioOut.h"

namespace {
	enum _inputs {	ANALOG_OUT_L_D=16, ANALOG_OUT_L, 
					ANALOG_OUT_R_D, ANALOG_OUT_R, 
					USB_OUT_L_D, USB_OUT_L, 
					USB_OUT_R_D, USB_OUT_R=23
					};
};

//CONSTRUCTOR
AudioOut :: AudioOut(const Address& a)
	:	Module(a)
	,	_volume_pot0(pins::VOLUME)
	,	analogOutL(a, ANALOG_OUT_L, ANALOG_OUT_L_D, _analogOut, 0, "ANALOG_OUT_L")
	,	analogOutR(a, ANALOG_OUT_R, ANALOG_OUT_R_D, _analogOut, 1, "ANALOG_OUT_R")
	,	usbOutL(a, USB_OUT_L, USB_OUT_L_D, _usbOut, 0, "USB_OUT_L")
	,	usbOutR(a, USB_OUT_R, USB_OUT_R_D, _usbOut, 1, "USB_OUT_R")
{

	_audioCtrl.enable();				   
	_audioCtrl.volume(0.5);
	_volume_pot0.setRange(0, 0.8, EXP);
}

void
AudioOut :: updateValues() {
	if(_volume_pot0.wasUpdated()){
		_audioCtrl.volume(_volume_pot0.read());
	}
	return;
}