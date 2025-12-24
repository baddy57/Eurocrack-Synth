#include "FX_Multi.h"

#include "../controls/Potentiometer.h"
#include "../controls/Switch.h"
#include "../controls/Button.h"

#include "../controls/OutputSocket.h"
#include "../controls/InputSocket.h"

#include <vector>
#include <Audio.h>
#include <cstdint>

namespace {
	enum _inputs {};
	enum _outputs {};
};

//ctor
FX_Multi :: FX_Multi (const Address& a)
:	//init list
	Module(a),
	_sw0	(a, 0), 
	_sw1	(a, 1),
	_pot0	(a, 2),
	_btn0	(a, 3),
	stream0(), 
	stream1()
{
	
	outputSockets.push_back(std::make_shared<OutputSocket>(a, id___, dectid____, @@stream@@, ___, "name___"));
	inputSockets.push_back(std::make_shared<InputSocket>(a, id___, dectid____, @@stream@@, ___, "name___"));
	
	internalConns.push_back(new AudioConnection(stream0,0, stream1,0));
}

void
FX_Multi :: updateValues() {
	if(_pot0.wasUpdated()){
		stream0.foo(_pot0.f_read());
	}
	
	
	if(sw0[0].b_read()) 
		current_fx = new ...
	
}