#include "../test_config.h"
#include "../../core/module_type_ids.h"

// VCO_det configuration
static const TestControl vco_det_analog[] = {
	{"COARSE",   8, TestControlType::POTENTIOMETER, 3800.f},
	{"FINE",    10, TestControlType::POTENTIOMETER, 3200.f},
	{"FM_CV",   11, TestControlType::POTENTIOMETER, 0.f},
	{"PM_CV",   12, TestControlType::POTENTIOMETER, 0.f},
	{"AMP",     15, TestControlType::POTENTIOMETER, 0.f},
};

static const TestControl vco_det_digital[] = {
	{"FM/PM",    9, TestControlType::SWITCH,        0.f},
	{"FREQ_JK", 24, TestControlType::JACK_DETECTOR, 0.f},  // FREQ_PHASE_CV_D
	{"SHAP_JK", 13, TestControlType::JACK_DETECTOR, 0.f},  // SHAPE_CV_D
	{"SIN_JK",  25, TestControlType::JACK_DETECTOR, 0.f},  // SIN_OUT_D
	{"TRI_JK",  26, TestControlType::JACK_DETECTOR, 0.f},  // TRI_OUT_D
	{"SAW_JK",  27, TestControlType::JACK_DETECTOR, 0.f},  // SAW_OUT_D
	{"SQR_JK",  28, TestControlType::JACK_DETECTOR, 0.f},  // SQR_OUT_D
};

static const ModuleTestConfig VCO_DET_CONFIG = {
	ModuleTypeIds::VCO_ID, "VCO_det",
	vco_det_analog, 5,
	vco_det_digital, 7
};

// ADSR configuration
static const TestControl adsr_analog[] = {
	{"ATTACK",  12, TestControlType::POTENTIOMETER, 4700.f},  // ATT
	{"DECAY",   13, TestControlType::POTENTIOMETER, 4700.f},  // _DEC
	{"SUSTAIN", 24, TestControlType::POTENTIOMETER, 4700.f},  // SUS
	{"RELEASE", 25, TestControlType::POTENTIOMETER, 4700.f},  // _REL
};

static const TestControl adsr_digital[] = {
	{"GATE_JK", 31, TestControlType::JACK_DETECTOR, 0.f},  // GATE_D
	{"OUT_JK",  30, TestControlType::JACK_DETECTOR, 0.f},  // SIGNAL_OUT_D
};

static const ModuleTestConfig ADSR_CONFIG = {
	ModuleTypeIds::ADSR, "ADSR",
	adsr_analog, 4,
	adsr_digital, 2
};

// Mixer8 configuration - uses fix() macro values
// fix(39)=15, fix(38)=14, fix(37)=13, fix(36)=12, fix(35)=11, fix(34)=10, fix(33)=9, fix(32)=8
static const TestControl mixer8_analog[] = {
	{"GAIN0",   15, TestControlType::POTENTIOMETER, 4700.f},  // fix(39)
	{"GAIN1",   14, TestControlType::POTENTIOMETER, 4700.f},  // fix(38)
	{"GAIN2",   13, TestControlType::POTENTIOMETER, 4700.f},  // fix(37)
	{"GAIN3",   12, TestControlType::POTENTIOMETER, 4700.f},  // fix(36)
	{"GAIN4",   11, TestControlType::POTENTIOMETER, 4700.f},  // fix(35)
	{"GAIN5",   10, TestControlType::POTENTIOMETER, 4700.f},  // fix(34)
	{"GAIN6",    9, TestControlType::POTENTIOMETER, 4700.f},  // fix(33)
	{"GAIN7",    8, TestControlType::POTENTIOMETER, 4700.f},  // fix(32)
};

static const TestControl mixer8_digital[] = {
	{"CH0_JK",  16, TestControlType::JACK_DETECTOR, 0.f},  // CH0_D
	{"CH1_JK",  17, TestControlType::JACK_DETECTOR, 0.f},  // CH1_D
	{"CH2_JK",  18, TestControlType::JACK_DETECTOR, 0.f},  // CH2_D
	{"CH3_JK",  19, TestControlType::JACK_DETECTOR, 0.f},  // CH3_D
	{"CH4_JK",  20, TestControlType::JACK_DETECTOR, 0.f},  // CH4_D
	{"CH5_JK",  21, TestControlType::JACK_DETECTOR, 0.f},  // CH5_D
	{"CH6_JK",  22, TestControlType::JACK_DETECTOR, 0.f},  // CH6_D
	{"CH7_JK",  23, TestControlType::JACK_DETECTOR, 0.f},  // CH7_D
	{"OUTL_JK", 32, TestControlType::JACK_DETECTOR, 0.f},  // OUT_STEREO_L_D
	{"OUTR_JK", 33, TestControlType::JACK_DETECTOR, 0.f},  // OUT_STEREO_R_D
	{"OUTM_JK", 35, TestControlType::JACK_DETECTOR, 0.f},  // OUT_MONO_D
};

static const ModuleTestConfig MIXER8_CONFIG = {
	ModuleTypeIds::MIXER8, "Mixer8",
	mixer8_analog, 8,
	mixer8_digital, 11
};

// VCA configuration (no potentiometers)
static const TestControl vca_digital[] = {
	{"OUT0_JK",  0, TestControlType::JACK_DETECTOR, 0.f},  // OUT0_D
	{"IN0_JK",   2, TestControlType::JACK_DETECTOR, 0.f},  // IN0_D
	{"CV0_JK",   4, TestControlType::JACK_DETECTOR, 0.f},  // CV0_D
	{"IN1_JK",  27, TestControlType::JACK_DETECTOR, 0.f},  // IN1_D
	{"CV1_JK",  29, TestControlType::JACK_DETECTOR, 0.f},  // CV1_D
	{"OUT1_JK", 31, TestControlType::JACK_DETECTOR, 0.f},  // OUT1_D
};

static const ModuleTestConfig VCA_CONFIG = {
	ModuleTypeIds::VCA, "VCA",
	nullptr, 0,
	vca_digital, 6
};

// LFO configuration
static const TestControl lfo_analog[] = {
	{"WAVE",    24, TestControlType::SELECTOR_MULTI, 4700.f},  // SEL (5 positions)
	{"FREQ",    25, TestControlType::POTENTIOMETER,  4700.f},  // POT0
	{"CV_AMT",  26, TestControlType::POTENTIOMETER,  4700.f},  // POT1
};

static const TestControl lfo_digital[] = {
	{"CV_JK",   27, TestControlType::JACK_DETECTOR, 0.f},  // CV_D
	{"SYNC_JK", 29, TestControlType::JACK_DETECTOR, 0.f},  // SYNC_D
	{"OUT_JK",  31, TestControlType::JACK_DETECTOR, 0.f},  // OUT_D
};

static const ModuleTestConfig LFO_CONFIG = {
	ModuleTypeIds::LFO, "LFO",
	lfo_analog, 3,
	lfo_digital, 3
};

// VCF configuration
static const TestControl vcf_analog[] = {
	{"FREQ",    16, TestControlType::POTENTIOMETER, 4700.f},  // POT0
	{"RESO",    17, TestControlType::POTENTIOMETER, 3200.f},  // POT1
	{"OCTAVE",  18, TestControlType::POTENTIOMETER, 3200.f},  // POT2
};

static const TestControl vcf_digital[] = {
	{"IN_JK",   25, TestControlType::JACK_DETECTOR, 0.f},  // SIG_IN_D
	{"CV_JK",   27, TestControlType::JACK_DETECTOR, 0.f},  // FREQ_CV_D
	{"HPF_JK",  29, TestControlType::JACK_DETECTOR, 0.f},  // HPF_D
	{"BPF_JK",  30, TestControlType::JACK_DETECTOR, 0.f},  // BPF_D
	{"LPF_JK",  31, TestControlType::JACK_DETECTOR, 0.f},  // LPF_D
};

static const ModuleTestConfig VCF_CONFIG = {
	ModuleTypeIds::FILTER, "VCF",
	vcf_analog, 3,
	vcf_digital, 5
};

// MidiMono configuration
static const TestControl midi_mono_digital[] = {
	{"USB_SW",  24, TestControlType::SWITCH,        0.f},  // USBSW
	{"CH+",     25, TestControlType::BUTTON,        0.f},  // CHPLUS
	{"CH-",     26, TestControlType::BUTTON,        0.f},  // CHMINUS
	{"GATE_JK", 29, TestControlType::JACK_DETECTOR, 0.f},  // GATE_D
	{"CV_JK",   30, TestControlType::JACK_DETECTOR, 0.f},  // CV_D
	{"VEL_JK",  31, TestControlType::JACK_DETECTOR, 0.f},  // VEL_D
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
