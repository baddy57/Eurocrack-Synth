#include "AudioIn.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <Audio.h>
#include <cstdint>

#include "AudioOut.h"
//AudioControlSGTL5000 AudioOut::_audioCtrl;


namespace {
	enum _inputs {MIC_GAIN=16, MIC_IN_D, SW, LINE_IN_L_D, LINE_IN_R_D, USB_IN_L_D, USB_IN_R_D=23};
	enum _outputs {LINE_IN_L=2, MIC_IN, LINE_IN_R, USB_IN_L, USB_IN_R=7};
};


AudioIn :: AudioIn(const Address& a) 
	:	Module(a)
	,	_lineIn()
	,	_usbIn()
	, _micGain(a, 16, 4700)
{
	//MIC TODO
	//_audioCtrl.inputSelect(AUDIO_INPUT_LINEIN);

	outputSockets.push_back(std::make_shared<OutputSocket>(a, LINE_IN_L, LINE_IN_L_D, _lineIn, 0, "LINE_IN_L"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, LINE_IN_R, LINE_IN_R_D, _lineIn, 1, "LINE_IN_R"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, USB_IN_L, USB_IN_L_D, _usbIn, 0, "USB_IN_L"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, USB_IN_R, USB_IN_R_D, _usbIn, 1, "USB_IN_R"));
}		

void AudioIn :: updateValues() {
	//_audioCtrl.inputSelect(AUDIO_INPUT_LINEIN);
	//_audioCtrl.inputSelect(AUDIO_INPUT_MIC);
	
	//todo mic gain
	return;
}