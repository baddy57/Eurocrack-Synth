#pragma once

#include <Audio.h>
#include <cstdint>

class GateIn : public AudioRecordQueue{
	public:
		GateIn();
		bool read();
	private:
		bool _isOpen;
		bool isAvailable(){return available()>0;};
};