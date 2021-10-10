#ifndef __CONTROL_H__
#define __CONTROL_H__ 1

#include "Address.h"
#include <cstdint>




class Control{
	protected:
		ControlAddress address;
		bool _wasUpdated;
	public:
		Control() = delete;
		Control(const Address& a, uint_fast8_t id) : address(/* new ControlAddress( */a, id), _wasUpdated(true){};
		Control(uint_fast8_t pin) : address(pin), _wasUpdated(true){};
		
	//	virtual uint_fast8_t 	i_read(){}; //integer read
	//	virtual float 	f_read(){}; //float read
	//	virtual bool 	b_read(){}; //boolean read
	//	virtual void 	update()=0; //20200822_0702
		
		// bool wasUpdated() {update(); return _wasUpdated;};
};

#endif