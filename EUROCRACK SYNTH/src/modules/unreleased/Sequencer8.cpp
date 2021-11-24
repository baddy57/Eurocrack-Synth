#include "Sequencer8.h"

namespace {
	const uint FDR_PINS[] = { 0,0,0,0,0,0,0,0 };
	const int FADER_PULLUP = 0;
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
Sequencer8::Sequencer8(const Address&a)
	:	Module(a)
{
	for (int i = 0; i < 8; ++i) {
		fader[i] = new Fader(a, FDR_PINS[i], FADER_PULLUP);
		fader[i]->setRange(0, 12);
	}
	clock_time = 0;
	currentStep = 0;
}

void Sequencer8::updateValues()
{
	if external clock connected
		getClockFromInput
	else if pot clock was updated
		setfrompot

	update selector
	if selector == setnote
		for(8) {
			if step[i].fader->wasupdated
				step[i].note = scale(root, fader read)
				step[i].freq = notes::freq[note, octave]
		}
	return;

	uint btn_id 
	if (!btn_id= whatButtonIsPressed();) return //no btn is pressed
	switch selector
		case scale
		scale = scales[btn_id]

		case oct
		oct = btn_id



	if button_plus was pressed
		switch(selpos)
			case scale if scale< scale++

	if button_cycle was pressed
		switch(selectorPosition)
			case SCALE_SELECTION: scale = (scale+1)%totscales

	if button_minus was pressed
		switch (selectorPosition)
			case SCALE_SELECTION
}
