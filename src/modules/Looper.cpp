#include "Looper.h"

namespace {
	//enum _inputs {	IN, IN_D,					CLOCK,	CLOCK_D,					_B0, _B1, _B2, _B3,					OUT_D	};
	const uint 
		IN = 29, IN_D = 28,
		CLOCK = 9, CLOCK_D = 8,
		_BTN[TRACKS_COUNT] = { 10,12,11,13,27,25 },
		_B0 = 10, _B1 = 12,
		_B2 = 11, _B3 = 13, 
		_B4 = 27, _B5 = 25,
		OUT = 6, OUT_D = 30;

	//enum _outputs {OUT};
	enum loopStates {IDLE, RECORDING, PLAYING, STOPPED, QUEUED};
};
int LoopTrack::longest = 0;

//ctor
Looper::Looper(const Address& a)
	:	Module(a)
	,	in(a, IN, IN_D, recorder, 0, "LOOPER IN")
	,	out(a, OUT, OUT_D, masterMixer, 0, "LOOPER OUT")
{
	for (int i = 0; i < TRACKS_COUNT; ++i) {
		buttons[i] = new Button2t(a, _BTN[i]);
		internalConns.push_back(new AudioConnection(recorder, 0, loops[i].recorder, 0));
		loops[i].filename[0] = 'l';
		loops[i].filename[1] = 'o';
		loops[i].filename[2] = 'o';
		loops[i].filename[3] = 'p';
		loops[i].filename[4] = i + 48;
		loops[i].filename[5] = '.';
		loops[i].filename[6] = 'R';
		loops[i].filename[7] = 'A';
		loops[i].filename[8] = 'W';
		loops[i].filename[9] = '\0';
		loops[i].state = IDLE;
	}

	internalConns.push_back(new AudioConnection(loops[0].player, 0, mixer_a, 0));
	internalConns.push_back(new AudioConnection(loops[1].player, 0, mixer_a, 1));
	internalConns.push_back(new AudioConnection(loops[2].player, 0, mixer_a, 2));
	internalConns.push_back(new AudioConnection(loops[3].player, 0, mixer_a, 3));
	internalConns.push_back(new AudioConnection(loops[4].player, 0, mixer_b, 0));
	internalConns.push_back(new AudioConnection(loops[5].player, 0, mixer_b, 1));
	internalConns.push_back(new AudioConnection(mixer_a, 0, masterMixer, 0));
	internalConns.push_back(new AudioConnection(mixer_b, 0, masterMixer, 1));
	internalConns.push_back(new AudioConnection(recorder, 0, masterMixer, 2));
	
	for (int i = 0; i < 4; ++i) {
		mixer_a.gain(i, 0.4);
		mixer_b.gain(i, 0.4);
		masterMixer.gain(i, 0.8);
	}
	recorder.gain(1.f);
}

void
Looper :: updateValues() {
	for (int i = 0; i < TRACKS_COUNT; ++i) {
		LoopTrack& loop = loops[i];
		//if (loop.state == PLAYING)
			//loop.continuePlay();
		if (buttons[i]->tap()) {

			switch (loop.state)
			{
			case IDLE:
				loop.startRec();
				break;

			case RECORDING:
				loop.stopRec();
				loop.enqueue();
				queuedCount++;
				break;

			case PLAYING:
				loop.stop();
				playingCount--;
				break;

			case STOPPED:
				if (playingCount == 0) {
					loop.play();
					playingCount++;
				}
				else {
					loop.enqueue();
					queuedCount++;
				}
				break;

			default:break;
			}
		}

		if (buttons[i]->longPress()) {
			switch (loop.state)
			{
			case IDLE:
				break;

			case RECORDING:
				loop.cancelRec();
				break;

			case PLAYING:
				loop.overdub();
				break;

			case STOPPED:
				loop.deleteRec();
				break;

			default:break;
			}
		}

		if (buttons[i]->doubleTap()) {
			switch (loop.state)
			{
			case IDLE:
				break;
			case RECORDING:
				break;
			case PLAYING:
				break;
			default:break;
			}
		}

		if (playingCount==0 && queuedCount == 1) {
			loop.play();
			playingCount++;
			queuedCount--;
		}
		//play all the tracks if  longest track has finished playing
		if (loop.trackLength == LoopTrack::longest && loop.state == PLAYING && !loop.player.isPlaying()) {
			loop.play();
			for (uint j = 0; j < TRACKS_COUNT; j++) {
				if(loops[j].state==QUEUED)
				loops[j].play();
				queuedCount--;
				playingCount++;
			}
		}

		//enqueue all the non-longest tracks that have finished playing
		if (loop.trackLength < LoopTrack::longest && loop.state == PLAYING && !loop.player.isPlaying()) {
			loop.state = QUEUED;
			queuedCount++;
			playingCount--;
		}

		if (loop.state == RECORDING)
			loop.continueRec();

		//////////////////
		///known issue: if the longest track is deleted, all the others remain stuck in the queue and cannot be deleted
		//////////////////
#ifdef disable
		//sync tracks
		if (queuedCount > 0) {
			for (int i = 0; i < 4; ++i) {
				if (loops[i].state == QUEUED) {
					if (loops[i].trackLength > LoopTrack::longest)
						delay(0);
					else
						delay(LoopTrack::longest - i);
					loops[i].play();
				}
			}
		}
#endif
	}
}
void LoopTrack::startRec() {
	Serial.printf("recording loop %i \n",filename[4]);
	
	if (SD.exists(filename))
		SD.remove(filename);
	file = SD.open(filename, FILE_WRITE);
	recorder.begin();
	state = RECORDING;
}
void LoopTrack::continueRec() {
	if (recorder.available() >= 2) {
		byte buffer[512];
		memcpy(buffer, recorder.readBuffer(), 256);
		recorder.freeBuffer();
		memcpy(buffer + 256, recorder.readBuffer(), 256);
		recorder.freeBuffer();
		// write all 512 bytes to the SD card
		//elapsedMicros usec = 0;
		file.write(buffer, 512);
	}
}
void LoopTrack::cancelRec()
{
	recorder.end();
	recorder.freeBuffer();
	SD.remove(filename);
	file.close();
	state = IDLE;
}

void LoopTrack::stopRec()
{
	Serial.printf("saving loop %i \n", filename[4]);

	recorder.end();
	while (recorder.available() > 0) {
		file.write((byte*)recorder.readBuffer(), 256);
		recorder.freeBuffer();
	}
	file.close();
	state = IDLE;
}

void LoopTrack::enqueue()
{
	Serial.printf("loop %i in queue ", filename[4]);

	player.play(filename);
	trackLength = player.lengthMillis();
	player.stop();

	Serial.printf("of length = %i \n", trackLength);

	if (trackLength > longest)
		longest = trackLength;
	state = QUEUED;
}

void LoopTrack::play()
{
	Serial.printf("playing loop %i ", filename[4]);
	Serial.printf("of length = %i \n", trackLength);
	player.play(filename);
	state = PLAYING;
}

void LoopTrack::stop()
{
	Serial.printf("stopped loop %i \n", filename[4]);
	player.stop();
	state = STOPPED;
}

void LoopTrack::deleteRec()
{
	Serial.printf("deleted loop %i \n", filename[4]);
	SD.remove(filename);
	file.close();
	trackLength = 0;
	state = IDLE;
}

void LoopTrack::overdub()
{
}

