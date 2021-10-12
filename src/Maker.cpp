#include "Maker.h"

#include "HardwareCfg.h"
#include "Module.h"
#include "Address.h"
#include "controls/IdMux.h"

#include "modules/EmptyModule.h"
#include "modules/AudioOut.h"
#include "modules/AudioIn.h"
#include "modules/VCA.h"
#include "modules/VCO_det.h"
#include "modules/Mixer8.h"
#include "modules/MidiMono.h"
#include "modules/DrumMachine.h"
#include "modules/Vcf.h"
#include "modules/ADSR.h"

#include <cstdint>
#include <vector>

extern const uint_fast8_t MAX_MODULES;
extern ILI9341_t3 tft;

/* namespace{
	enum moduleType {
		zero, 	//0 = 0000 0000
		AUDIO_OUT,		//1 = 0000 0001
		AUDIO_IN,		//2 = 0000 0010
		MIDI_MODULE,	//3 = 0000 0011
		VCO_ID = 4,			//4 = 0000 0100
		LFO,
		FILTER,
		ADSR,
		VCA,
		MIXER8,  //9
		DISTORTION,
		REVERB,
		DELAY,
		LOOPER,
		NOISE,
		FLANGER,
		AUTOTUNE,
		DRUMS,
		INSTRUMENTS,
		EMPTY_MODULE = 255
	};
}; */

//array of pointers to foos(constructors)?
	//case VCO return new constructors[VCO]
//no switch case?
	//return new constructors[type]
uint_fast8_t connected_modules =0;

void
Maker :: createModules (std::vector<Module*>& vm) {	
	for (uint_fast8_t i=0; i<MAX_MODULES; ++i)
		vm.push_back(factory(Address(i)));
	tft.print("total modules connected: ");
	tft.println(connected_modules);
}

uint_fast8_t
Maker :: getModuleID(const Address& moduleAddress) {
	IdMux idMux(moduleAddress);
	uint_fast8_t res(idMux.read());
	// if(res!=0){
		// tft.print(" - id ");
		// tft.print(res);
	// }
	return res;
}

//factory method
Module*
Maker :: factory (const Address& slotAddress) {
//	tft.println("Maker :: factory");
	uint_fast8_t moduleID = getModuleID(slotAddress);
	uint_fast8_t type = 0;
	for(int i=0; i<8; ++i)
		bitWrite(type, i, bitRead(moduleID, i));
	
	bool verbose = true;
	
	switch(type){
		case 0:{
		//	tft.println("empty" );
		//	tft.println("EMPTY" );
			return new EmptyModule(slotAddress);
			break;
		}
		 
		case 1:{
			if(verbose){
				tft.print(type);
				tft.print("  AUDIO_OUT" );
				tft.print("  @  " );
				tft.println(slotAddress.toInt());	
			}
			++connected_modules;
			return new AudioOut(slotAddress);
			break;
		}
		case 2:{
			if(verbose){
				tft.print(type);
				tft.print("  AUDIO_IN" );
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new AudioIn(slotAddress);
			break;
		}
		case 3:{
			if(verbose){
				tft.print(type);
				tft.print("  VCF" );
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new VCF(slotAddress);
			break;
		}
		case 4:{
			if(verbose){
				tft.print(type);
				tft.print("  VCO_det" );
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new VCO_det(slotAddress);
			break;
		}
		case 5:{
			if(verbose){
				tft.print(type);
				tft.print("  MIDI MONO");
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new MidiMono(slotAddress);
			break;
		}
		case 6:{
			if(verbose){
				tft.print(type);
				tft.print("  ADSR");
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new ADSR(slotAddress);
			break;
		}
		case 9:{
			if(verbose){
				tft.print(type);
				tft.print("  MIXER8");
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new Mixer8(slotAddress);
			break;
		}
		case 10:{
			if(verbose){
				tft.print(type);
				tft.print("  VCA");
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new VCA(slotAddress);
			break;
		}
		case 11:{
			if(verbose){
				tft.print(type);
				tft.print("  DRUMZ");
				tft.print("  @  " );
				tft.println(slotAddress.toInt());
			}
			++connected_modules;
			return new DrumMachine(slotAddress);
			break;
		}
		default :{
		 	// tft.print(type);
			// tft.print("  unknown" );
		 	// tft.print("  @  " );
		 	// tft.println(slotAddress.toInt());
			return new EmptyModule(slotAddress);
			break;
		}
	};
}
