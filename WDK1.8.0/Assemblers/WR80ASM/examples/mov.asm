define .NULL 0
define .NaN  #$

macro .mov _reg1, _reg2
	if DR
		if #_reg2 == DR
			ld #_reg1
		endf
		else
			if #_reg2 != .NaN
				std #_reg2
			endf
			else
				stl #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			if #_reg2 == .NULL
				cdr
				ld #_reg1
			endf
			else
				std #_reg2
				ld #_reg1
			ende
		endf
		else
			stl #_reg2
			ld #_reg1
		ende
	ende
endm

.mov DR, 2
.mov DR, r0
.mov r1, DR
.mov r1, r0
.mov r2, .NULL
.mov r3, 1

