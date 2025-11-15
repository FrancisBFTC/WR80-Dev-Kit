define .NaN #$

macro test _val1, _val2
	if #1 == .NaN
		db 0
	endf
	else
		db 1
	ende
endm

test 3, 2
