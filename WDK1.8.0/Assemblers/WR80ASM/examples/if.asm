macro test ...
	;define DR 3
	if !DR
		st 1
	endf
	if DR == #2
		st 2
	endf
	st 4
endm

test 2, DR

label:
if !label
	if label == 65535
		and r2
	endf
	
	label:
	
	if label != 65535
		and r1
	endf
endf

define val1 3
define val2 2

if val1 ^ val2
	and r3
endf



