#pragma once

// #include <vector>
#include <cstdint>
#include <vector>

#include "../core/address.h"
#include "../core/module.h"
#include "../core/hardware_cfg.h"

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

	static void factory();
	static void updateAll();

private:
	static void logModuleDetected(uint_fast8_t moduleType, String moduleName, const Address &slotAddress);
};
