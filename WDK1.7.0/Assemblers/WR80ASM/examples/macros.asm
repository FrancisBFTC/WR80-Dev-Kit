macro .mov _num1, _num2
	if #1 == DR
		if #2 != DR
			stl #_num2
		endif
	endif
	if #2 == DR
		if #1 != DR
			ld #_num1
		endif
	endif
	if !DR
		stl #_num2
		ld #_num1
	endif
endm

macro .mov 1
	std #1::8
	ld r0
	std #1::0
endm

macro .mov
	st 0
endm

macro .read ...
	st #1
endm

.mov r3, 10
.mov r1, DR
.mov r1, r0
.mov $FF01
.mov r2, r0
.mov $FF02
.mov r3, r0
.mov $FF03
.mov r4, r0
.mov $FF04
.mov r1, r4
.mov $FE01
.mov r2, r5
.mov $FE02
.mov r3, r6
.mov $FE03
.mov r4, r7
.mov $FE04
.mov
.mov

.read 3, 4, 5
.read 2
