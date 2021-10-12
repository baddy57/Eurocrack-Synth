#ifndef __PATCH_CABLE_H__
#define __PATCH_CABLE_H__

// #include "CONTROLS_InputSocket_POLY.h"
// #include "CONTROLS_OutputSocket_POLY.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "HardwareCfg.h"

#include <Audio.h>
//#include <list>

// class OutputSocket;
// class InputSocket;
// class OutputSocket_POLY;
// class InputSocket_POLY;

class PatchCable {
	private:
		InputSocket* _in;
		OutputSocket* _out;
		AudioConnection* ac[POLYPHONY];
		uint_fast8_t connectionType;
		void linkSockets(OutputSocket*, InputSocket*);
	public:
		//ctor
		#ifndef POLYPHONIC
			PatchCable(OutputSocket*, InputSocket*);
		#endif
		#ifdef POLYPHONIC
			//ctor MONO TO MONO
			PatchCable(OutputSocket*, InputSocket*);
			//ctor MONO TO POLY
			PatchCable(OutputSocket*, InputSocket_POLY*);
			//ctor POLY TO MONO
			PatchCable(OutputSocket_POLY*, InputSocket*);
			//ctor POLY TO POLY
			PatchCable(OutputSocket_POLY*, InputSocket_POLY*);
		#endif
		//dtor
		~PatchCable();
		
		InputSocket* getInputSocket() const {return _in;};
		OutputSocket* getOutputSocket() const {return _out;};
		
		static std::list <PatchCable*> activeConnections;
		static void updateCables();
		static bool checkConnection (const OutputSocket*, const InputSocket*);
		
		bool operator==(const PatchCable& other) const {return this->_in==other.getInputSocket()&&this->_out==other.getOutputSocket();};
};

#endif