#include "AudioIn.h"

namespace {
	enum _inputs {MIC_GAIN=16, MIC_IN_D, SW, LINE_IN_L_D, LINE_IN_R_D, USB_IN_L_D, USB_IN_R_D=23};
	enum _outputs {LINE_IN_L=2, MIC_IN, LINE_IN_R, USB_IN_L, USB_IN_R=7};
};

AudioIn :: AudioIn(const Address& a) 
	:	Module(a)
	,   _micGain(a, 16, 4700)
{
	//MIC TODO
	//_audioCtrl.inputSelect(AUDIO_INPUT_LINEIN);
	_audioCtrl.adcHighPassFilterDisable(); //http://prajtm.scripts.mit.edu/projects/portfolio/portable-synth-v1-prototype/
	_audioCtrl.lineInLevel(1);
	//bool x = _audioCtrl.inputSelect(AUDIO_INPUT_MIC);

	outputSockets.push_back(std::make_shared<OutputSocket>(a, LINE_IN_L, LINE_IN_L_D, _preamp, 0, "LINE_IN_L"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, LINE_IN_R, LINE_IN_R_D, _lineIn, 1, "LINE_IN_R"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, USB_IN_L, USB_IN_L_D, _usbIn, 0, "USB_IN_L"));
	outputSockets.push_back(std::make_shared<OutputSocket>(a, USB_IN_R, USB_IN_R_D, _usbIn, 1, "USB_IN_R"));

	internalConns.push_back(new AudioConnection(_lineIn, 0, _preamp, 0));
	_preamp.gain(10);
}		

void AudioIn :: updateValues() {
	//_audioCtrl.inputSelect(AUDIO_INPUT_LINEIN);
	//_audioCtrl.inputSelect(AUDIO_INPUT_MIC);
	
	//todo mic gain
	return;
}