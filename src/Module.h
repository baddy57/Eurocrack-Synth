#ifndef __MODULE_H__
#define __MODULE_H__ 1

#include "Address.h"
#include "Control.h"
#include "CONTROLS_InputSocket.h"
#include "CONTROLS_OutputSocket.h"
#include <vector>
#include <Audio.h>


class Module{
	private:
				
	protected:
		Address moduleAddress;
		std :: vector <InputSocket*> inputSocket;
		std :: vector <OutputSocket*> outputSocket;
		std :: vector <AudioConnection*> internalConns;
		bool verbose;
	public:
		Module(const Address&);
		
		void updateConnections();
		virtual void updateValues()=0; 
};

#endif