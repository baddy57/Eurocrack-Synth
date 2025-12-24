#ifndef __GATE_IN_H__
#define __GATE_IN_H__

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
#endif // !__GATE_IN_H__