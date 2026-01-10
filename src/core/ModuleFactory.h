#pragma once

// #include <vector>
#include <cstdint>
#include <vector>

#include "Address.h"
#include "Module.h"
#include "HardwareCfg.h"

#include "ModuleTypeIds.h"
#include "../controls/ModuleTypeIdMux.h"

#include "../modules/EmptyModule.h"
#include "../modules/AudioOut.h"
#include "../modules/AudioIn.h"
#include "../modules/VCA.h"
#include "../modules/LFO.h"
#include "../modules/VCO_det.h"
#include "../modules/Mixer8.h"
#include "../modules/MidiMono.h"
#include "../modules/DrumMachine.h"
#include "../modules/Vcf.h"
#include "../modules/ADSR.h"
#include "../modules/Looper.h"
#include "../modules/fx/Reverb.h"
#include "../modules/fx/Distortion_amp.h"
#include "../modules/fx/Distortion_bc.h"
#include "../modules/fx/Delay_single.h"
#include "../modules/fx/Delay_multi.h"

extern const uint_fast8_t MAX_MODULES;
extern ILI9341_t3 Serial;

class ModuleFactory
{
public:
	static void factory(std::vector<Module *> &activeModules)
	{
		Serial.println("init modules");

		for (uint_fast8_t i = 0; i < MAX_MODULES; ++i)
		{
			Address slotAddress(i);

			uint_fast8_t moduleType = ModuleTypeIdMux(slotAddress).getModuleId();

			switch (moduleType)
			{

			case 0: break;

			case ModuleTypeIds::AUDIO_OUT:
			{
				logModuleDetected(moduleType, "AUDIO_OUT", slotAddress);
				activeModules.push_back(new AudioOut(slotAddress));
				break;
			}
			case ModuleTypeIds::AUDIO_IN:
			{
				logModuleDetected(moduleType, "AUDIO_IN", slotAddress);
				activeModules.push_back(new AudioIn(slotAddress));
				break;
			}
			case ModuleTypeIds::FILTER:
			{
				logModuleDetected(moduleType, "VCF", slotAddress);
				activeModules.push_back(new VCF(slotAddress));
				break;
			}
			case ModuleTypeIds::VCO_ID:
			{
				logModuleDetected(moduleType, "VCO_det", slotAddress);
				activeModules.push_back(new VCO_det(slotAddress));
				break;
			}
			case ModuleTypeIds::MIDI_MODULE:
			{
				logModuleDetected(moduleType, "MIDI_MONO", slotAddress);
				activeModules.push_back(new MidiMono(slotAddress));
				break;
			}
			case ModuleTypeIds::ADSR:
			{
				logModuleDetected(moduleType, "ADSR", slotAddress);
				activeModules.push_back(new ADSR(slotAddress));
				break;
			}
			case ModuleTypeIds::MIXER8:
			{
				logModuleDetected(moduleType, "MIXER 8", slotAddress);
				activeModules.push_back(new Mixer8(slotAddress));
				break;
			}
			case ModuleTypeIds::VCA:
			{
				logModuleDetected(moduleType, "VCA", slotAddress);
				activeModules.push_back(new VCA(slotAddress));
				break;
			}
			case ModuleTypeIds::DRUMS:
			{
				logModuleDetected(moduleType, "DRUMZ", slotAddress);
				activeModules.push_back(new DrumMachine(slotAddress));
				break;
			}
			case ModuleTypeIds::DELAY:
			{
				logModuleDetected(moduleType, "DELAY MULTI", slotAddress);
				activeModules.push_back(new Delay_multi(slotAddress));
				break;
			}
			case ModuleTypeIds::DISTORTION:
			{
				logModuleDetected(moduleType, "DISTORTION_bc", slotAddress);
				activeModules.push_back(new Distortion_bc(slotAddress));
				break;
			}
			case ModuleTypeIds::REVERB:
			{
				logModuleDetected(moduleType, "REVERB", slotAddress);
				activeModules.push_back(new Reverb(slotAddress));
				break;
			}
			case ModuleTypeIds::DISTORTION_AMP:
			{
				logModuleDetected(moduleType, "DISTORTION_amp", slotAddress);
				activeModules.push_back(new Distortion_amp(slotAddress));
				break;
			}
			case ModuleTypeIds::DELAY_SINGLE:
			{
				logModuleDetected(moduleType, "DELAY_SINGLE", slotAddress);
				activeModules.push_back(new Delay_single(slotAddress));
				break;
			}
			case ModuleTypeIds::LOOPER:
			{
				logModuleDetected(moduleType, "LOOPER", slotAddress);
				activeModules.push_back(new Looper(slotAddress));
				break;
			}
			case ModuleTypeIds::LFO:
			{
				logModuleDetected(moduleType, "LFO", slotAddress);
				activeModules.push_back(new LFO(slotAddress));
				break;
			}
			default:
			{
				logModuleDetected(moduleType, "UNKNOWN", slotAddress);
				break;
			}
			};

			if (slotAddress.toInt() % 16 == 15)
				Serial.println("---------end of bank-------------");
		}

		Serial.println("----------------------------------------");
	}

	static void logModuleDetected(uint_fast8_t moduleType, String moduleName, const Address &slotAddress)
	{
		bool verbose = true;

		if (!verbose)
			return;

		Serial.print("Module detected: ");
		Serial.print(moduleType);
		Serial.print("  ");
		Serial.print(moduleName);
		Serial.print("  @  ");
		Serial.println(slotAddress.toInt());
	}
};