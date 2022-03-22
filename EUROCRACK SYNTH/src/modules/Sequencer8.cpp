#include "Sequencer8.h"

namespace {
	const uint FDR_PINS[8] = { 0,0,0,0,0,0,0,0 };
	const uint BTN_PINS[8] = { 0,0,0,0,0,0,0,0 };
	const uint ROTSW_PINS[6] = { 0,0,0,0,0,0 };
	const uint POT0 = 0;
	const uint CLOCK_IN = 0;
	const uint CLOCK_D = 0;
	const uint GATE_OUT = 0;
	const uint GATE_D = 0;
	const uint CV_OUT = 0;
	const uint CV_D = 0;
	const uint VEL_OUT = 0;
	const uint VEL_D = 0;
	const int FADER_PULLUP = 0;
	enum {SET_VELOCITY, SET_OCTAVE, SET_PROBABILITY, SET_DELAY, SET_LENGTH, SET_SCALE};
	const float VELOCITY[8]{ 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1 };
	const float PROBABILITY[8]{ 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1 };
	const float DELAY[8]{ 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1 };
	const float LENGTH[8]{ 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1 };
	enum SCALES{MINOR, MAJOR, BLUES, PENTA_MINOR, PENTA_MAJOR};
}

void Sequencer8::getClockFromInput()
{
	if (clockInReader.read())
		tempo_ms = timer - lastBeat;
}
void Sequencer8::playStep()
{
	//gate = is auto
	//cv = currentstep.note
	//vel = currentstep.vel
	if (currentStep == 8)
		currentStep = 0;
	else currentStep++;
	return;
}
Sequencer8::Sequencer8(const Address& a)
	: Module(a)
	, fnSelector(a, 6, ROTSW_PINS)
	, manualClock(a, POT0)
	, clockIn(a, CLOCK_IN, CLOCK_D, clockInReader, 0, "seq8 clock in")
	, gateOut(a, GATE_OUT, GATE_D, gateStream, 0, "seq8 gate out")
	, cvOut(a, CV_OUT, CV_D, cvStream, 0, "seq8 cv out")
	, velocityOut(a, VEL_OUT, VEL_D, velocityStream, 0, "seq8 vel out")
{
	for (int i = 0; i < 8; ++i) {
		faders[i] = new Fader(a, FDR_PINS[i], FADER_PULLUP);
		faders[i]->setRange(0, 12);
		buttons[i] = new Button(a, BTN_PINS[i]);
	}

	manualClock.setRange(0, 0, 0);
	currentStep = 0;
}

void Sequencer8::updateValues()
{
	//if external clock connected
	if (clockIn.base->isReady()) {
		//getClockFromInput
		sequenceLength_ms = 0;
	}
	else if (manualClock.wasUpdated()) {
		bpm = manualClock.read();
		sequenceLength_ms = 0;
	}

	//first tap
	if (chosenStep != -1)
		for (int i = 0; i < 8; ++i) {
			if (buttons[i]->tap()) {
				chosenStep = i;
				break;
			}
		}
	//second tap
	else
		for (int i = 0; i < 8; ++i) {
			if (buttons[i]->tap()) {
				stepParam = i;
				break;
			}
		}

	//first tap = step id, second tap = value, sel pos = fn
	//move selector to cancel key press 
	if (chosenStep != -1 && stepParam != -1) {
		//update selector; 
		if (!fnSelector.wasUpdated()) {
			switch (fnSelector.read()) {
			case SET_VELOCITY:
				steps[chosenStep].velocity = VELOCITY[stepParam];
				break;
			case SET_OCTAVE:
				steps[chosenStep].octave = stepParam;
				break;
			case SET_PROBABILITY:
				steps[chosenStep].probability = PROBABILITY[stepParam];
				break;
			case SET_DELAY:
				steps[chosenStep].delay = DELAY[stepParam];
				break;
			case SET_LENGTH:
				steps[chosenStep].length = LENGTH[stepParam];
				break;
			case SET_SCALE:
				switch (chosenStep)
				{
				case MAJOR:
					scale = scales::majorScale;
					break;
				default:
					break;
				}
				//scale = SCALES[stepParam];
				for (int i = 0; i < 8; ++i) {
					steps[i].note = scale(root, faders[i]->read());
					steps[i].modified = true;
				}
				break;
			}
		}
		steps[chosenStep].modified = true;
		chosenStep = -1;
		stepParam = -1;
	}
	
	//update notes
	for (int i = 0; i < 8; ++i) {
		if (faders[i]->wasUpdated()) {
			steps[i].note = scale(root,faders[i]->read()); //should also be triggered when scale is changed
			steps[i].modified = true;
		}
		if (steps[i].modified) {
			steps[i].frequency = notes::freq[steps[i].note][steps[i].octave];
			steps[i].modified = false;
			steps[i].attackTime_ms = i * sequenceLength_ms / 8 + steps[i].delay;
			steps[i].releaseTime_ms = steps[i].attackTime_ms + steps[i].length;
		}
	}

	timer += millis()% sequenceLength_ms;
	//actual sequencing
	if (timer  == 0) {
		gateStream.amplitude(0);
		timer = 0;
	}

	for (int i = 8; i > 0; --i) {
		if (timer > steps[i].releaseTime_ms) {
			gateStream.amplitude(0);
			break;
		}
		if (timer > steps[i].attackTime_ms) {
			cvStream.amplitude(steps[i].frequency);
			gateStream.amplitude(1);
			velocityStream.amplitude(steps[i].velocity);
			break;
		}
	}
		//stert sequence
	return;

}
