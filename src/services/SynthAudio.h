#pragma once

#include <Audio.h>

// Note: AudioMemory() macro must be called from main.cpp
// because it requires a compile-time constant

class SynthAudio {
public:
	static AudioControlSGTL5000 codec;

	static void enable();
	static void setVolume(float vol);
};
