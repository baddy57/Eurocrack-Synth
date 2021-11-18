#ifndef __MODULES_Looper_H__
#define __MODULES_Looper_H__

#include "../Module.h"
const int TRACKS_COUNT = 6;

struct LoopTrack {
	AudioPlaySdRaw player;
	AudioRecordQueue recorder;
	File file;
	char filename[10];
	int trackLength = 0;
	static int longest;
	uint state;
	void startRec();
	void continueRec();
	void cancelRec(); 
	void stopRec();
	void enqueue();
	void play();
	//void continuePlay();
	void stop();
	void deleteRec();
	void overdub();
};

class Looper : public Module {
private:
	int playingCount=0;
	int queuedCount=0;
	Button2t* buttons[TRACKS_COUNT];
	LoopTrack loops[TRACKS_COUNT];
	AudioMixer4 mixer_a, mixer_b, masterMixer;
	AudioAmplifier recorder;
public:
	Looper (const Address&);
	void updateValues();
};

#endif