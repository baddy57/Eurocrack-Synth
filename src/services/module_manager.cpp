#include "module_manager.h"

// Static member definition
std::vector<Module*> ModuleManager::activeModules;

void ModuleManager::factory()
{
	for (uint_fast8_t i = 0; i < MAX_MODULES; ++i)
	{
		Address slotAddress(i);

		uint_fast8_t moduleType = ModuleTypeIdMux(slotAddress).getModuleId();

		Module* module = nullptr;

		switch (moduleType)
		{

		case 0: break;

		case ModuleTypeIds::AUDIO_OUT:
		{
			module = new AudioOut(slotAddress);
			break;
		}
		case ModuleTypeIds::AUDIO_IN:
		{
			module = new AudioIn(slotAddress);
			break;
		}
		case ModuleTypeIds::FILTER:
		{
			module = new VCF(slotAddress);
			break;
		}
		case ModuleTypeIds::VCO_ID:
		{
			module = new VCO_det(slotAddress);
			break;
		}
		case ModuleTypeIds::MIDI_MODULE:
		{
			module = new MidiMono(slotAddress);
			break;
		}
		case ModuleTypeIds::ADSR:
		{
			module = new ADSR(slotAddress);
			break;
		}
		case ModuleTypeIds::MIXER8:
		{
			module = new Mixer8(slotAddress);
			break;
		}
		case ModuleTypeIds::VCA:
		{
			module = new VCA(slotAddress);
			break;
		}
		case ModuleTypeIds::DRUMS:
		{
			module = new DrumMachine(slotAddress);
			break;
		}
		case ModuleTypeIds::DELAY:
		{
			module = new Delay_multi(slotAddress);
			break;
		}
		case ModuleTypeIds::DISTORTION:
		{
			module = new Distortion_bc(slotAddress);
			break;
		}
		case ModuleTypeIds::REVERB:
		{
			module = new Reverb(slotAddress);
			break;
		}
		case ModuleTypeIds::DISTORTION_AMP:
		{
			module = new Distortion_amp(slotAddress);
			break;
		}
		case ModuleTypeIds::DELAY_SINGLE:
		{
			module = new Delay_single(slotAddress);
			break;
		}
		case ModuleTypeIds::LOOPER:
		{
			module = new Looper(slotAddress);
			break;
		}
		case ModuleTypeIds::LFO:
		{
			module = new LFO(slotAddress);
			break;
		}
		default:
		{
			break;
		}
		};

		if (module != nullptr) {
			logModuleDetected(moduleType, module->getModuleName(), slotAddress);
			activeModules.push_back(module);
		}

		#if CONFIGURATION__LOGGER__MODULE_FACTORY
		if (MAX_MODULES > 16 && slotAddress.getId() % 16 == 15)
			Serial.printf("---------end of bank %i-------------\n", i);
		#endif
	}

	#if CONFIGURATION__LOGGER__MODULE_FACTORY
	tft.println("----------------------------------------");
	#endif
}

void ModuleManager::logModuleDetected(uint_fast8_t moduleType, String moduleName, const Address &slotAddress)
{
	#if CONFIGURATION__LOGGER__MODULE_FACTORY
	tft.print("Module detected: ");
	tft.print(moduleType);
	tft.print("  ");
	tft.print(moduleName);
	tft.print("  @  ");
	tft.println(slotAddress.getId());
	#endif
}

void ModuleManager::updateAll() {
	for (auto* module : activeModules) {
		module->updateValues();
	}
}
