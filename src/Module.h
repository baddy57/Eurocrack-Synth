#ifndef __MODULE_H__
#define __MODULE_H__

#include "Address.h"
#include "Control.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include <Audio.h>
#include <vector>
#include <memory>
#include <list>
extern ILI9341_t3 tft;


class Module{
	private:
				
	protected:
		Address moduleAddress;
		std :: vector <AudioConnection*> internalConns;
		bool verbose;
	public:
		Module(const Address&);
		static std::vector<std::shared_ptr<InputSocket>> inputSockets;
		static std::vector<std::shared_ptr<OutputSocket>> outputSockets;

		virtual void updateValues()=0; 
		static void updateConnections();
};

#endif