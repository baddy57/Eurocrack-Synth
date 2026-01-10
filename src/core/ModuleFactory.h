#pragma once

// #include <vector>
#include <cstdint>
#include <vector>

#include "Address.h"
#include "Module.h"
#include "HardwareCfg.h"

#include "ModuleIdentifiers.h"
#include "../controls/IdMux.h"

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

			uint_fast8_t moduleType = IdMux(slotAddress).getModuleId();

			switch (moduleType)
			{

			case 0: break;

			case ModuleIdentifiers::AUDIO_OUT:
			{
				logModuleDetected(moduleType, "AUDIO_OUT", slotAddress);
				activeModules.push_back(new AudioOut(slotAddress));
				break;
			}
			case ModuleIdentifiers::AUDIO_IN:
			{
				logModuleDetected(moduleType, "AUDIO_IN", slotAddress);
				activeModules.push_back(new AudioIn(slotAddress));
				break;
			}
			case ModuleIdentifiers::FILTER:
			{
				logModuleDetected(moduleType, "VCF", slotAddress);
				activeModules.push_back(new VCF(slotAddress));
				break;
			}
			case ModuleIdentifiers::VCO_ID:
			{
				logModuleDetected(moduleType, "VCO_det", slotAddress);
				activeModules.push_back(new VCO_det(slotAddress));
				break;
			}
			case ModuleIdentifiers::MIDI_MODULE:
			{
				logModuleDetected(moduleType, "MIDI_MONO", slotAddress);
				activeModules.push_back(new MidiMono(slotAddress));
				break;
			}
			case ModuleIdentifiers::ADSR:
			{
				logModuleDetected(moduleType, "ADSR", slotAddress);
				activeModules.push_back(new ADSR(slotAddress));
				break;
			}
			case ModuleIdentifiers::MIXER8:
			{
				logModuleDetected(moduleType, "MIXER 8", slotAddress);
				activeModules.push_back(new Mixer8(slotAddress));
				break;
			}
			case ModuleIdentifiers::VCA:
			{
				logModuleDetected(moduleType, "VCA", slotAddress);
				activeModules.push_back(new VCA(slotAddress));
				break;
			}
			case ModuleIdentifiers::DRUMS:
			{
				logModuleDetected(moduleType, "DRUMZ", slotAddress);
				activeModules.push_back(new DrumMachine(slotAddress));
				break;
			}
			case ModuleIdentifiers::DELAY:
			{
				logModuleDetected(moduleType, "DELAY MULTI", slotAddress);
				activeModules.push_back(new Delay_multi(slotAddress));
				break;
			}
			case ModuleIdentifiers::DISTORTION:
			{
				logModuleDetected(moduleType, "DISTORTION_bc", slotAddress);
				activeModules.push_back(new Distortion_bc(slotAddress));
				break;
			}
			case ModuleIdentifiers::REVERB:
			{
				logModuleDetected(moduleType, "REVERB", slotAddress);
				activeModules.push_back(new Reverb(slotAddress));
				break;
			}
			case ModuleIdentifiers::DISTORTION_AMP:
			{
				logModuleDetected(moduleType, "DISTORTION_amp", slotAddress);
				activeModules.push_back(new Distortion_amp(slotAddress));
				break;
			}
			case ModuleIdentifiers::DELAY_SINGLE:
			{
				logModuleDetected(moduleType, "DELAY_SINGLE", slotAddress);
				activeModules.push_back(new Delay_single(slotAddress));
				break;
			}
			case ModuleIdentifiers::LOOPER:
			{
				logModuleDetected(moduleType, "LOOPER", slotAddress);
				activeModules.push_back(new Looper(slotAddress));
				break;
			}
			case ModuleIdentifiers::LFO:
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