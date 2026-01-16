#pragma once

#include "../core/module.h"

namespace VCA_pins {
	enum outputs { OUT0, OUT1 };
	enum inputs { OUT0_D, IN0, IN0_D, CV0, CV0_D,
				  IN1_D=27, IN1, CV1_D, CV1, OUT1_D };
}

class VCA : public Module {
private:
	AudioEffectMultiply _amp0, _amp1;

	ModuleOutput out1;
	ModuleInput in1;
	ModuleInput cv1;

public:
	inline VCA(const Address& a)
		: Module(a)
		, out1(a, VCA_pins::OUT1, VCA_pins::OUT1_D, _amp1, 0, "VCA_1_out")
		, in1(a, VCA_pins::IN1, VCA_pins::IN1_D, _amp1, 0, "VCA_1_in")
		, cv1(a, VCA_pins::CV1, VCA_pins::CV1_D, _amp1, 1, "VCA_1_cv")
	{
	}

	inline void updateValues() override {
	}

	uint8_t getModuleTypeId() const override { return ModuleTypeIds::VCA; }
	
	const char* getModuleName() const override { return "VCA"; }

	#if TEST_MODE_ENABLED
	void getTestControls(
		std::vector<TestControlInfo>& analog,
		std::vector<TestControlInfo>& digital) override
	{
		// VCA has no analog controls or digital controls
		// Jack detectors are shown in socket section
	}

	void getTestSockets(std::vector<TestSocketInfo>& sockets) override {
		sockets.push_back(TestSocketInfo::createInput(in1.base));
		sockets.push_back(TestSocketInfo::createInput(cv1.base));
		sockets.push_back(TestSocketInfo::createOutput(out1.base));
	}
	#endif
};
