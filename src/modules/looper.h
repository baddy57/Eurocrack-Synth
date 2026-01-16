#pragma once

#include "../core/module.h"
#include "../features/loop_track.h"

class Looper : public Module {
private:
	int playingCount = 0;
	int queuedCount = 0;
	Button2t* buttons[TRACKS_COUNT];
	LoopTrack loops[TRACKS_COUNT];
	AudioMixer4 mixer_a, mixer_b, masterMixer;
	AudioAmplifier recorder;
	ModuleInput in;
	ModuleOutput out;

public:
	Looper() = delete;

	inline Looper(const Address& a)
		: Module(a)
		, in(a, Looper_pins::IN, Looper_pins::IN_D, recorder, 0, "LOOPER IN")
		, out(a, Looper_pins::OUT, Looper_pins::OUT_D, masterMixer, 0, "LOOPER OUT")
	{
		using namespace Looper_pins;

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

	inline void updateValues() override {
		using namespace Looper_pins;

		for (int i = 0; i < TRACKS_COUNT; ++i) {
			LoopTrack& loop = loops[i];

			if (buttons[i]->tap()) {
				switch (loop.state) {
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
				switch (loop.state) {
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
				switch (loop.state) {
				case IDLE:
				case RECORDING:
				case PLAYING:
				default:break;
				}
			}

			if (playingCount == 0 && queuedCount == 1) {
				loop.play();
				playingCount++;
				queuedCount--;
			}

			// Play all the tracks if longest track has finished playing
			if (loop.trackLength == LoopTrack::longest && loop.state == PLAYING && !loop.player.isPlaying()) {
				loop.play();
				for (uint j = 0; j < TRACKS_COUNT; j++) {
					if (loops[j].state == QUEUED)
						loops[j].play();
					queuedCount--;
					playingCount++;
				}
			}

			// Enqueue all the non-longest tracks that have finished playing
			if (loop.trackLength < LoopTrack::longest && loop.state == PLAYING && !loop.player.isPlaying()) {
				loop.state = QUEUED;
				queuedCount++;
				playingCount--;
			}

			if (loop.state == RECORDING)
				loop.continueRec();
		}
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::LOOPER; }
	
	const char* getModuleName() const override { return "Looper"; }
	
	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		//todo
	}

	void getTestSockets(std::vector<TestSocketInfo>& sockets) override {
		sockets.push_back(TestSocketInfo::createInput(in.base));
		sockets.push_back(TestSocketInfo::createOutput(out.base));
	}
	#endif
};
