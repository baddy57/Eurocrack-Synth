#include <Audio.h>
#include <cstdint>

class GateIn : public AudioRecordQueue{
	public:
		GateIn();
		bool isAvailable(){return available()>0;};
		bool read();
	private:
		bool _isOpen;
};