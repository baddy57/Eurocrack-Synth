# basics
this is an arduino project based on a teensy 4.1 board.
it uses the audio library by paul stoffregen to implement an eurorack style modular synthesizer emulator.
the key difference with the real thing is that the audio is all processed inside the (single) teensy board which is located on a mother board pcb, while the modules only act as a controller. 
no actual audio flows in the patch cables.
the large number of required pins is achieved via cascaded 4051 (or 4053) multiplexers. 

# electrical implementation details
up to 4 cases (also referred to as racks or banks) can be connected to the motherboard via db25 connectors and each case has 16 idc 16pin connectors (slots) to which modules can be connected.

a module only consists of a pcb with a bunch of controls (potentiometers, sockets, buttons etc). unlike a real eurorack synth, the idc 16pin connector transports signals from those controls back to the teensy (instead of just power)

modules are identified via a 4051 multiplexer with high and low pins representing an 8 bit number (module type id).
slots are identified by an address that is a number that in its binary representation has each digit meaning the high/low value of a certain selector pin (a,b,c) of a certain multiplexer

## pins
- one read pin for all read operations, including detecting patch cable connections, analog reads, buttons etc
- one write pin for the ping sent by output sockets
- a shit ton of write pins that set the multiplexer selectors

## multiplexers
also referred to as mux
4051 multiplexers are used to multiply the available pins. they are used in cascade and there are two cascades, one for the read pin, one for the write pin. each 4051 has 3 binary selectors (2^3=8) which are labeled as SEL_<x> where x is a letter.
there are four level of multiplexers:
- mobo level 1 (selectors a,b,c)
- mobo level 2 (selectors d,e,f)
- rack level 1 (selectors g,h,i)
- rack level 2 (selectors j,k,l)
- module level 1 (selectors m,n,o)
- module level 2 (selectors p,q,r) - read only + used only in big modules

note: small modules instead of the pqr 4051 use a 4053 multiplexer in order to multiply the read pins by 4

## jack detectors
3.5mm sockets that are used have a pin that can be used to detect the physical presence of a jack. basically it acts like a switch

## addresses
### slot address
each slot has an address composed of 12 bits representing the values of selectors a to l that must be set in order to reach it

### control address (including input sockets)
each module has 24 pins available for controls (8*4 - 8 taken by the id mux)
each control has a local id (0-23) that represents the pin
the slot address + the pin address form the control address
jack detectors are considered controls

### output socket address 
same as the inputs, but only 8 pins are available, so the address has 3 less bits.

## module type id multiplexer
also called id mux, it is a 4051 mux located on each module. its 8 pins are interpreted as a binary number. each number corresponds to a certain module type.

# software implementation details
## module factory
reads the id mux on each available slot. if a moduletype id with the same value exists, the corresponding module is instanciated with the slot address passed as a parameter.

## modules
modules are based on audio library audio streams. multiple audio streams are combined in some cases to obtain more complex modules

## update values
quite straightforward. each control is polled and if its value changed, the corresponding action is executed (ie audio stream parameter is updated

## check connections
the output socket sends a signal through the cable. input sockets are polled to see if they are receiving the signal. if they do, a virtual patch cable is instanciated between the output and the input(s).
only "active" sockets are polled for better performance

## polyphony
polyphony is used when you play multiple notes (ie chords) on the same sound source
example:
    midi multi: gate1+note1 gate2+note2 gate3+note3

    notes go to a vco cv input. (normally it would take 1 oscillator per voice)
    gates go to an adsr envelope generator. (normally it would take 1 module per voice)

    polyphonic setting:

    midi module
        - 1x poly out (note)
        - 1x poly out (gate)
        - maybe velocity

    vco module with 4 internal vcos 
        - 1x poly in socket for freq
        - mono in sockets for other params (if any)
        - 1x poly out socket

    poly adsr:
        - 1x poly in
        - 1x poly out

    poly vca
        - 1x poly in socket
        - internal mixer + mono out? → no need for p2m

    output | input | description
    mono | mono | ok
    poly | poly | ok
    poly | mono | only `out[0]` is connected to `in`. other voices are lost 
    mono | poly | `out` is connected to `in[0]`

## lifetime
modules are instanciated at startup, along with all their controls and sockets. they are never destroyed.
patch cables are instanciated when the user plugs a physical cable on two valid sockets. they are destroyed when one end is unplugged

# todo board
todo board https://github.com/users/baddy57/projects/1/views/6


