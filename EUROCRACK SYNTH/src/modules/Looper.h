#ifndef __MODULES_Looper_H__
#define __MODULES_Looper_H__

#include "../Module.h"
#define MS_TO_BYTES

struct LoopTrack {
	AudioPlaySdRaw player;
	AudioRecordQueue recorder;
	File file;
	char filename[5];
	int trackLength = 0;
	static int longest;
	uint state;
	void startRec();
	void cancelRec();
	void stopRec();
	void enqueue();
	void play();
	void stop();
	void deleteRec();
	void overdub();
};

class Looper : public Module {
private:
	int playingCount;
	int queuedCount;
	Button2t* buttons[4];
	LoopTrack loops[4];
	AudioMixer4 masterMixer;
	AudioAmplifier recorder;
public:
	Looper (const Address&);
	void updateValues();
		
protected:
	
};

#endif