#include "Looper.h"

namespace {
	enum _inputs {	IN, IN_D,
					CLOCK,	CLOCK_D,
					_B0, _B1, _B2, _B3,
					OUT_D	};
	enum _outputs {OUT};
	enum loopStates {IDLE, RECORDING, PLAYING, MUTED, STOPPED, QUEUED, TOSYNC};
};
int LoopTrack::longest = 0;

//ctor
Looper::Looper(const Address& a)
	: Module(a)
{
	buttons[0] = new Button2t(a, _B0);
	buttons[1] = new Button2t(a, _B1);
	buttons[2] = new Button2t(a, _B2);
	buttons[3] = new Button2t(a, _B3);

	outputSockets.push_back(std::make_shared<OutputSocket>(a, OUT, OUT_D, masterMixer, 0, "LOOPER OUT"));
	
	inputSockets.push_back(std::make_shared<InputSocket>(a, IN, IN_D, recorder, 0, "LOOPER IN"));

	for (int i = 0; i < 4; ++i) {
		internalConns.push_back(new AudioConnection(loops[i].player, 0, masterMixer, i));
		internalConns.push_back(new AudioConnection(recorder, 0, loops[i].recorder, 0));
		loops[i].filename[4] = i;
		masterMixer.gain(i, 0.4);
	}

	
}

void
Looper :: updateValues() {
	for (int i = 0; i < 4; ++i) {
		LoopTrack& loop = loops[i];
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
			case MUTED:
				break;
			default:break;
			}
		}

		//play all the tracks if  longest track has finished playing
		if (loop.trackLength == LoopTrack::longest && loop.state == PLAYING && !loop.player.isPlaying()) {
			loop.play();
			for (int j = 0; j < 4; j++) {
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

	if (SD.exists(filename))
		SD.remove(filename);
	recorder.begin();
	state = RECORDING;
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
	recorder.end();
	file = SD.open(filename, FILE_WRITE);
	while (recorder.available() > 0) {
		file.write((byte*)recorder.readBuffer(), 256);
		recorder.freeBuffer();
	}
	file.close();
	state = IDLE;
}

void LoopTrack::enqueue()
{
	player.play(filename);
	trackLength = player.lengthMillis();
	player.stop();
	if (trackLength > longest)
		longest = trackLength;
	state = QUEUED;
}

void LoopTrack::play()
{
	player.play(filename);
	state = PLAYING;
}

void LoopTrack::stop()
{
	player.stop();
	state = STOPPED;
}

void LoopTrack::deleteRec()
{
	SD.remove(filename);
	file.close();
	trackLength = 0;
	state = IDLE;
}

void LoopTrack::overdub()
{
}
