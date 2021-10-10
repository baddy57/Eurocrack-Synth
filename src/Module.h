#ifndef __MODULE_H__
#define __MODULE_H__ 1

#include "Address.h"
#include "Control.h"
#include "controls/InputSocket.h"
#include "controls/OutputSocket.h"
#include <Audio.h>
#include <vector>


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