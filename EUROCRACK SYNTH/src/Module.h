#ifndef __MODULE_H__
#define __MODULE_H__

#include <memory>
#include <vector>
#include <cstdint>

#include <Audio.h>

#include "list"
#include "Address.h"
#include "Control.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include "controls/Potentiometer.h"
#include "controls/Button.h"
#include "controls/Button2t.h"
#include "controls/Switch.h"

extern ILI9341_t3 tft;

class Module{
	private:
				
	protected:
		Address moduleAddress;
		std :: vector <AudioConnection*> internalConns;
		bool verbose;
	public:
		Module(const Address&);

		static AudioControlSGTL5000 _audioCtrl;

		static std::vector<std::shared_ptr<InputSocket>> inputSockets;
		static std::vector<std::shared_ptr<OutputSocket>> outputSockets;

		//static unsigned int fix(const unsigned int);
		virtual void updateValues()=0; 
		static void updateConnections();
};

#endif