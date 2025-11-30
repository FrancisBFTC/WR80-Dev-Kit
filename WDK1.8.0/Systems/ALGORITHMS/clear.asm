
Start:
	call TestCarry
	jc PrintA
	jp PrintB
	
TestCarry:
	st 15
	shl 4
	st 15

	ld r0
	ld r1
	ld r2
	out p0

	cdr
	;clr
	bt r0
	jz EnableCarry
	
DisableCarry:
	dc
ret
EnableCarry:
	ec
ret

PrintA:
	st 6
	shl 4
	st 1
	out p3
	jp end
	
PrintB:
	st 6
	shl 4
	st 2
	out p3
	jp end
	
end:
	jp end

