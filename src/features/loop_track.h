#pragma once

#include "../core/module.h"
#include <Audio.h>
#include <SD.h>

const int TRACKS_COUNT = 6;

namespace LoopStates{

    enum { IDLE, RECORDING, PLAYING, STOPPED, QUEUED };
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
		Serial.printf("recording loop %i \n", filename[4]);

		if (SD.exists(filename))
			SD.remove(filename);
		file = SD.open(filename, FILE_WRITE);
		recorder.begin();
		state = LoopStates::RECORDING;
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
		recorder.end();
		recorder.freeBuffer();
		SD.remove(filename);
		file.close();
		state = LoopStates::IDLE;
	}

	inline void stopRec() {
		Serial.printf("saving loop %i \n", filename[4]);

		recorder.end();
		while (recorder.available() > 0) {
			file.write((byte*)recorder.readBuffer(), 256);
			recorder.freeBuffer();
		}
		file.close();
		state = LoopStates::IDLE;
	}

	inline void enqueue() {
		Serial.printf("loop %i in queue ", filename[4]);

		player.play(filename);
		trackLength = player.lengthMillis();
		player.stop();

		Serial.printf("of length = %i \n", trackLength);

		if (trackLength > longest)
			longest = trackLength;
		state = LoopStates::QUEUED;
	}

	inline void play() {
		Serial.printf("playing loop %i ", filename[4]);
		Serial.printf("of length = %i \n", trackLength);
		player.play(filename);
		state = LoopStates::PLAYING;
	}

	inline void stop() {
		Serial.printf("stopped loop %i \n", filename[4]);
		player.stop();
		state = LoopStates::STOPPED;
	}

	inline void deleteRec() {
		Serial.printf("deleted loop %i \n", filename[4]);
		SD.remove(filename);
		file.close();
		trackLength = 0;
		state = LoopStates::IDLE;
	}

	inline void overdub() {
		// Empty for now
	}
};
