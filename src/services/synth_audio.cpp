#include "synth_audio.h"

// Static member definition
AudioControlSGTL5000 SynthAudio::codec;

void SynthAudio::enable() {
	codec.enable();
}

void SynthAudio::setVolume(float vol) {
	codec.volume(vol);
}
