#pragma once

// #include <vector>
#include <cstdint>
#include <vector>

#include "../core/address.h"
#include "../core/module.h"
#include "../core/hardware_cfg.h"

#include "../core/module_type_ids.h"
#include "../controls/module_type_id_mux.h"

#include "../modules/empty_module.h"
#include "../modules/audio_out.h"
#include "../modules/audio_in.h"
#include "../modules/VCA.h"
#include "../modules/lfo.h"
#include "../modules/VCO_det.h"
#include "../modules/mixer8.h"
#include "../modules/midi_mono.h"
#include "../modules/drum_machine.h"
#include "../modules/Vcf.h"
#include "../modules/ADSR.h"
#include "../modules/Looper.h"
#include "../modules/Reverb.h"
#include "../modules/Distortion_amp.h"
#include "../modules/Distortion_bc.h"
#include "../modules/Delay_single.h"
#include "../modules/Delay_multi.h"

extern const uint_fast8_t MAX_MODULES;

#include "synth_display.h"
#define tft SynthDisplay::raw()

class Module;

class ModuleManager {
public:
	static std::vector<Module*> activeModules;

	/// @brief creates module instances for all detected modules and adds them to the provided vector
	static void factory()
	{
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

			#if CONFIGURATION__LOGGER__MODULE_FACTORY
			if (MAX_MODULES > 16 && slotAddress.toInt() % 16 == 15)
				Serial.printf("---------end of bank %i-------------\n", i);
			#endif
		}

		#if CONFIGURATION__LOGGER__MODULE_FACTORY
		tft.println("----------------------------------------");
		#endif
	}

	static void logModuleDetected(uint_fast8_t moduleType, String moduleName, const Address &slotAddress)
	{
		#if CONFIGURATION__LOGGER__MODULE_FACTORY
		tft.print("Module detected: ");
		tft.print(moduleType);
		tft.print("  ");
		tft.print(moduleName);
		tft.print("  @  ");
		tft.println(slotAddress.toInt());
		#endif
	}

	static void updateAll();
};
