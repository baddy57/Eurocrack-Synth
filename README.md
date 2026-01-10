# notes
mutliplexers are also referred to as mux

# polyphony
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

# todo board
todo board https://github.com/users/baddy57/projects/1/views/6


