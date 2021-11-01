#ifndef __MAKER_H__
#define __MAKER_H__

//#include <vector>
#include <cstdint>
#include <vector>

#include "Address.h"
#include "Module.h"
#include "HardwareCfg.h"
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


extern const uint_fast8_t MAX_MODULES;
extern ILI9341_t3 tft;

namespace module {
	namespace types {
		enum moduleTypeList {
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
	}


void factory(std::vector<Module*>& activeModules) {
	bool verbose = true;
	
	Serial.println("init modules");


	while (activeModules.size() == 0) {
	//while(true){
		for (uint_fast8_t i = 0; i < MAX_MODULES; ++i) {
			Address slotAddress(i);
			uint_fast8_t moduleType = IdMux(slotAddress).getModuleId();
			Serial.println(moduleType);
			switch (moduleType) {
			case 0: {
				break;
			}

			case 1: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  AUDIO_OUT");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new AudioOut(slotAddress));
				break;
			}
			case 2: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  AUDIO_IN");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new AudioIn(slotAddress));
				break;
			}
			case 3: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  VCF");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCF(slotAddress));
				break;
			}
			case 4: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  VCO_det");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCO_det(slotAddress));
				break;
			}
			case 5: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  MIDI MONO");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new MidiMono(slotAddress));
				break;
			}
			case 6: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  ADSR");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new ADSR(slotAddress));
				break;
			}
			case 9: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  MIXER8");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new Mixer8(slotAddress));
				break;
			}
			case 10: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  VCA");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCA(slotAddress));
				break;
			}
			case 11: {
				if (verbose) {
					tft.print(moduleType);
					tft.print("  DRUMZ");
					tft.print("  @  ");
					tft.println(slotAddress.toInt());
				}
				activeModules.push_back(new DrumMachine(slotAddress));
				break;
			}
			default: {
				break;
			}
			};

		}
		tft.println("----------------------------------------");
	}
}
}; 








#endif