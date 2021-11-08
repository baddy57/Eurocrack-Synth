# Eurocrack-Synth
inspired by https://github.com/mattybrad/polymod

this is my clone of an eurorack modular synthesizer.
it works based on a teensy board and uses paul stoffregen's audio library and other arduino libraries

basically there are a bunch of cascaded multiplexers, which are used to get as many things as possible connected to the board
up to 4 cases can be connected with a db25 cable to the main box, and each of them can have up to 16 modules connected via 16pin flat cables.
each one of these 64 slots has an address, corresponding to the multiplexer's selector values used to reach them.
every module has an unique id (set via dip switches) and when the teensy is turned on, it reads this id to understand which modules are connected and where.
modules can have up to 8 3.5mm sockets that can send signals to other modules and up to 24 (small pcb) or 56 (big pcb) among potentiometers, switches, buttons, faders and 3.5mm sockets for receiving signals.
(the actual number of the inputs in lower because both input and outputs sockets use two pins)

any output socket can be connected to multiple inputs using 3.5mm mono jacks and splitters
the signal that flows in these cables is not an audio signal, but just a digital signal sent from the outputs, that, if received by any inputs, lets the code know that an audioconnection object must be created.

similarities to real eurorack synths:
	module size, which is 128.5 x 5.08*n mm
	m3 screws used to attach the modules to the rack
	3.5mm cables
	fpc16 cables

other features: 
	analog audio in/out, thanks to the teensy audio board \n
	usb audio in/out \n
	midi in/out
	usb midi in/out
	a touch screen display (used mainly for debugging purposes at the moment)
	of course, rgb leds (i still need to fix them)

special thanks to:
	Paul Stoffregen and anybody who has worked on the audio library
	everyone on the pjrc forum
	youtube channel notes and volts
	again mattybrad for sharing his code