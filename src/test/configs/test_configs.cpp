#include "../test_config.h"
#include "../../core/module_type_ids.h"

// VCO_det configuration
// Ranges from vco_det.h: coarse 0-3200 EXP, fine -25-75 EXP, amp 0-1 EXP, fm/pm cv 0-12/0-9000
static const TestControl vco_det_analog[] = {
	{"COARSE",   8, TestControlType::POTENTIOMETER, 3800.f, 0.f, 3200.f, "Hz"},
	{"FINE",    10, TestControlType::POTENTIOMETER, 3200.f, -25.f, 75.f, "Hz"},
	{"FM_CV",   11, TestControlType::POTENTIOMETER, 0.f, 0.f, 12.f, nullptr},
	{"PM_CV",   12, TestControlType::POTENTIOMETER, 0.f, 0.f, 9000.f, nullptr},
	{"AMP",     15, TestControlType::POTENTIOMETER, 0.f, 0.f, 1.f, nullptr},
};

static const TestControl vco_det_digital[] = {
	{"FM/PM",    9, TestControlType::SWITCH,        0.f, 0.f, 0.f, nullptr},
	{"FREQ_JK", 24, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"SHAP_JK", 13, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"SIN_JK",  25, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"TRI_JK",  26, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"SAW_JK",  27, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"SQR_JK",  28, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig VCO_DET_CONFIG = {
	ModuleTypeIds::VCO_ID, "VCO_det",
	vco_det_analog, 5,
	vco_det_digital, 7
};

// ADSR configuration
// Ranges from adsr.h: attack/decay/release 0-10000 EXP ms, sustain 0-1 EXP
static const TestControl adsr_analog[] = {
	{"ATTACK",  12, TestControlType::POTENTIOMETER, 4700.f, 0.f, 10000.f, "ms"},
	{"DECAY",   13, TestControlType::POTENTIOMETER, 4700.f, 0.f, 10000.f, "ms"},
	{"SUSTAIN", 24, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"RELEASE", 25, TestControlType::POTENTIOMETER, 4700.f, 0.f, 10000.f, "ms"},
};

static const TestControl adsr_digital[] = {
	{"GATE_JK", 31, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUT_JK",  30, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig ADSR_CONFIG = {
	ModuleTypeIds::ADSR, "ADSR",
	adsr_analog, 4,
	adsr_digital, 2
};

// Mixer8 configuration - uses fix() macro values
// Ranges from mixer8.h: gain 0-1 EXP
static const TestControl mixer8_analog[] = {
	{"GAIN0",   15, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN1",   14, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN2",   13, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN3",   12, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN4",   11, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN5",   10, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN6",    9, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
	{"GAIN7",    8, TestControlType::POTENTIOMETER, 4700.f, 0.f, 1.f, nullptr},
};

static const TestControl mixer8_digital[] = {
	{"CH0_JK",  16, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH1_JK",  17, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH2_JK",  18, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH3_JK",  19, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH4_JK",  20, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH5_JK",  21, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH6_JK",  22, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CH7_JK",  23, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUTL_JK", 32, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUTR_JK", 33, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUTM_JK", 35, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig MIXER8_CONFIG = {
	ModuleTypeIds::MIXER8, "Mixer8",
	mixer8_analog, 8,
	mixer8_digital, 11
};

// VCA configuration (no potentiometers)
static const TestControl vca_digital[] = {
	{"OUT0_JK",  0, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"IN0_JK",   2, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CV0_JK",   4, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"IN1_JK",  27, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CV1_JK",  29, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUT1_JK", 31, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig VCA_CONFIG = {
	ModuleTypeIds::VCA, "VCA",
	nullptr, 0,
	vca_digital, 6
};

// LFO configuration
// Ranges from lfo.h: freq 0-20 EXP Hz, cv -5-5 LIN
static const TestControl lfo_analog[] = {
	{"WAVE",    24, TestControlType::SELECTOR_MULTI, 4700.f, 0.f, 4.f, nullptr},
	{"FREQ",    25, TestControlType::POTENTIOMETER,  4700.f, 0.f, 20.f, "Hz"},
	{"CV_AMT",  26, TestControlType::POTENTIOMETER,  4700.f, -5.f, 5.f, nullptr},
};

static const TestControl lfo_digital[] = {
	{"CV_JK",   27, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"SYNC_JK", 29, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"OUT_JK",  31, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig LFO_CONFIG = {
	ModuleTypeIds::LFO, "LFO",
	lfo_analog, 3,
	lfo_digital, 3
};

// VCF configuration
// Ranges from vcf.h: freq 20-3000 EXP Hz, reso 0.7-5 LIN, octave 0-7 LIN
static const TestControl vcf_analog[] = {
	{"FREQ",    16, TestControlType::POTENTIOMETER, 4700.f, 20.f, 3000.f, "Hz"},
	{"RESO",    17, TestControlType::POTENTIOMETER, 3200.f, 0.7f, 5.f, nullptr},
	{"OCTAVE",  18, TestControlType::POTENTIOMETER, 3200.f, 0.f, 7.f, nullptr},
};

static const TestControl vcf_digital[] = {
	{"IN_JK",   25, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CV_JK",   27, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"HPF_JK",  29, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"BPF_JK",  30, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"LPF_JK",  31, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig VCF_CONFIG = {
	ModuleTypeIds::FILTER, "VCF",
	vcf_analog, 3,
	vcf_digital, 5
};

// MidiMono configuration
static const TestControl midi_mono_digital[] = {
	{"USB_SW",  24, TestControlType::SWITCH,        0.f, 0.f, 0.f, nullptr},
	{"CH+",     25, TestControlType::BUTTON,        0.f, 0.f, 0.f, nullptr},
	{"CH-",     26, TestControlType::BUTTON,        0.f, 0.f, 0.f, nullptr},
	{"GATE_JK", 29, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"CV_JK",   30, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
	{"VEL_JK",  31, TestControlType::JACK_DETECTOR, 0.f, 0.f, 0.f, nullptr},
};

static const ModuleTestConfig MIDI_MONO_CONFIG = {
	ModuleTypeIds::MIDI_MODULE, "MidiMono",
	nullptr, 0,
	midi_mono_digital, 6
};

// Configuration registry
static const ModuleTestConfig* ALL_CONFIGS[] = {
	&VCO_DET_CONFIG,
	&ADSR_CONFIG,
	&MIXER8_CONFIG,
	&VCA_CONFIG,
	&LFO_CONFIG,
	&VCF_CONFIG,
	&MIDI_MONO_CONFIG,
	nullptr  // Terminator
};

const ModuleTestConfig* findTestConfig(uint8_t typeId) {
	for (int i = 0; ALL_CONFIGS[i] != nullptr; ++i) {
		if (ALL_CONFIGS[i]->moduleTypeId == typeId) {
			return ALL_CONFIGS[i];
		}
	}
	return nullptr;
}
