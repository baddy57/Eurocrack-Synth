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
#include "controls/SelectorMulti.h"

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

struct Input {
	inline  Input(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono in")
	{
		Module::inputSockets.push_back(std::make_shared<InputSocket>(a, id, detectorId, as, port, name));
	}

	inline Input(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly in")
	{
		Module::inputSockets.push_back(std::make_shared<InputSocket>(a, id, detectorId, as0, as1, as2, as3, port, name));
	}
};

struct Output {
	inline  Output(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as,
		uint_fast8_t port,
		const char* name = "mono out")
	{
		Module::outputSockets.push_back(std::make_shared<OutputSocket>(a, id, detectorId, as, port, name));
	}

	inline Output(const Address& a,
		uint_fast8_t id,
		uint_fast8_t detectorId,
		AudioStream& as0,
		AudioStream& as1,
		AudioStream& as2,
		AudioStream& as3,
		uint_fast8_t port,
		const char* name = "poly out")
	{
		Module::outputSockets.push_back(std::make_shared<OutputSocket>(a, id, detectorId, as0, as1, as2, as3, port, name));
	}
};
#endif