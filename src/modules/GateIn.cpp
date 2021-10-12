#include "GateIn.h"
#include <Audio.h>
#include <cstdint>

GateIn :: GateIn(): AudioRecordQueue(){
	_isOpen = false;
	this->begin();
}

bool
GateIn :: read(){
	if(this->available()>0){
		int16_t queue[128];
		memcpy(queue, this->readBuffer(), 256);
		this->freeBuffer();
		bool newGateReading = (queue[0]>0);
		if(newGateReading != _isOpen) {
			_isOpen = newGateReading;
			if(_isOpen)
				return true;
			else 
				return false;
		}
		clear();
	}
}