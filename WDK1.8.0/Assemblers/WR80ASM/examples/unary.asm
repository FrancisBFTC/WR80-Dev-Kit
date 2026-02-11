include "relat.asm"

define state0 wres1 >= 1 && wres1 <= 5
define state1 wres1 >= 1 && wres1 <= 8
define state2 (1 + 2 + 3 + 4 + 5) / 3 >= 5
define state3 -wsum
define state4 ~1
define state5 ~(-(-(-(state3)))
define state6 !(state0 ^ state1)
define state7 ~1 + ~1 + -1

std state0
std state1

if state2
	std state2
	if state2 == 1
		db 255
	endf
endf

std state3
std state4
std state5
std state6
db state7