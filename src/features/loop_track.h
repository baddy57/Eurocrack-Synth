#pragma once

#include "../core/module.h"
#include <Audio.h>
#include <SD.h>

const int TRACKS_COUNT = 6;

namespace Looper_pins {
	const uint
		IN = 29, IN_D = 28,
		CLOCK = 9, CLOCK_D = 8,
		_BTN[TRACKS_COUNT] = { 10, 12, 11, 13, 27, 25 },
		OUT = 6, OUT_D = 30;

	enum loopStates { IDLE, RECORDING, PLAYING, STOPPED, QUEUED };
}

struct LoopTrack {
	AudioPlaySdRaw player;
	AudioRecordQueue recorder;
	File file;
	char filename[10];
	int trackLength = 0;
	static int longest;
	uint state;

	inline void startRec() {
		using namespace Looper_pins;
		Serial.printf("recording loop %i \n", filename[4]);

		if (SD.exists(filename))
			SD.remove(filename);
		file = SD.open(filename, FILE_WRITE);
		recorder.begin();
		state = RECORDING;
	}

	inline void continueRec() {
		if (recorder.available() >= 2) {
			byte buffer[512];
			memcpy(buffer, recorder.readBuffer(), 256);
			recorder.freeBuffer();
			memcpy(buffer + 256, recorder.readBuffer(), 256);
			recorder.freeBuffer();
			file.write(buffer, 512);
		}
	}

	inline void cancelRec() {
		using namespace Looper_pins;
		recorder.end();
		recorder.freeBuffer();
		SD.remove(filename);
		file.close();
		state = IDLE;
	}

	inline void stopRec() {
		using namespace Looper_pins;
		Serial.printf("saving loop %i \n", filename[4]);

		recorder.end();
		while (recorder.available() > 0) {
			file.write((byte*)recorder.readBuffer(), 256);
			recorder.freeBuffer();
		}
		file.close();
		state = IDLE;
	}

	inline void enqueue() {
		using namespace Looper_pins;
		Serial.printf("loop %i in queue ", filename[4]);

		player.play(filename);
		trackLength = player.lengthMillis();
		player.stop();

		Serial.printf("of length = %i \n", trackLength);

		if (trackLength > longest)
			longest = trackLength;
		state = QUEUED;
	}

	inline void play() {
		using namespace Looper_pins;
		Serial.printf("playing loop %i ", filename[4]);
		Serial.printf("of length = %i \n", trackLength);
		player.play(filename);
		state = PLAYING;
	}

	inline void stop() {
		using namespace Looper_pins;
		Serial.printf("stopped loop %i \n", filename[4]);
		player.stop();
		state = STOPPED;
	}

	inline void deleteRec() {
		using namespace Looper_pins;
		Serial.printf("deleted loop %i \n", filename[4]);
		SD.remove(filename);
		file.close();
		trackLength = 0;
		state = IDLE;
	}

	inline void overdub() {
		// Empty for now
	}
};
