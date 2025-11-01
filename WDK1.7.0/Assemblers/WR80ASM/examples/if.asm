macro test ...
	;define DR 3
	if !DR
		st 1
	endif
	if DR == #2
		st 2
	endif
	st 4
endm

test 2, DR

label:
if !label
	if label == 65535
		and r2
	endif
	
	label:
	
	if label != 65535
		and r1
	endif
endif

define val1 3
define val2 2

if val1 ^ val2
	and r3
endif



