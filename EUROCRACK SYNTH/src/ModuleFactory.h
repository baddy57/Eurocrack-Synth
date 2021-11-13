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
#include "modules/fx/Reverb.h"
#include "modules/fx/Distortion_amp.h"
#include "modules/fx/Distortion_bc.h"
#include "modules/fx/Delay_single.h"
#include "modules/fx/Delay_multi.h"

extern const uint_fast8_t MAX_MODULES;
//extern ILI9341_t3 Serial;

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
			MIXER8,  //9 = 00001001
			DISTORTION,
			REVERB, //11 = 00001011
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
			//Serial.println(moduleType);
			switch (moduleType) {
			case 0: {
				//Serial.print("---");
				//Serial.println(slotAddress.toInt());
				break;
			}

			case 1: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  AUDIO_OUT");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new AudioOut(slotAddress));
				break;
			}
			case 2: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  AUDIO_IN");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new AudioIn(slotAddress));
				break;
			}
			case 3: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  VCF");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCF(slotAddress));
				break;
			}
			case 4: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  VCO_det");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCO_det(slotAddress));
				break;
			}
			case 5: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  MIDI MONO");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new MidiMono(slotAddress));
				break;
			}
			case 6: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  ADSR");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new ADSR(slotAddress));
				break;
			}
			case 9: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  MIXER8");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new Mixer8(slotAddress));
				break;
			}
			case 10: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  VCA");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new VCA(slotAddress));
				break;
			}
			case 11: {
				if (verbose) {
					Serial.print(moduleType);
					Serial.print("  DRUMZ");
					Serial.print("  @  ");
					Serial.println(slotAddress.toInt());
				}
				activeModules.push_back(new DrumMachine(slotAddress));
				break;
			}
			case 12: {
				Serial.print(moduleType);
				Serial.print("  Delay_multi");
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				activeModules.push_back(new Delay_multi(slotAddress));
				break;
			}
			case 13: {
				Serial.print(moduleType);
				Serial.print("  DISTORTION_bc");
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				activeModules.push_back(new Distortion_bc(slotAddress));
				break;
			}
			case 14: {
				Serial.print(moduleType);
				Serial.print("  REVERB");
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				activeModules.push_back(new Reverb(slotAddress));
				break;
			}
			case 15: {
				Serial.print(moduleType);
				Serial.print("  DISTORTION_amp");
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				activeModules.push_back(new Distortion_amp(slotAddress));
				break;
			}

			case 16: {
				Serial.print(moduleType);
				Serial.print("  Delay_single");
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				activeModules.push_back(new Delay_single(slotAddress));
				break;
			}
			default: {
				Serial.print(moduleType);
				Serial.print("  @  ");
				Serial.println(slotAddress.toInt());
				break;
			}
			};
			if (slotAddress.toInt() % 16 == 15) Serial.println("----------------------");
		}
		Serial.println("----------------------------------------");
	}
}
}; 








#endif