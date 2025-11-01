define value 2

macro .jne _label
	jz end##
	jp #_label
	end##:
endm

.jne code
.jne code
and r0
code:

std ##
if value == ##
	db 1
	if value != 2
		db 2
	endf
	else
		if value < 3
			db 4
		endf
	ende
endf
else
	db 0
ende

