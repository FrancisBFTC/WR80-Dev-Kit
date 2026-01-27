ORG 0

jp start

include "floatw8.asm"

start:
	call setfloatstr
	
	;std 1
	;ld r0
	std 64
	ld r1
	std 1
	div r1
	;push r1
	;push r0
	;call div8
	;pop r1
	;pop r1
	call EncodeFloat8
	;call PrintFloat8
	
	call ParseFloat8
	call setfloatstr
	call print
	
	pushb
	pops
	std 2
	ssp
	
	dc
.END

print:
	std 0x01
	idc
	loop.prt:
		call checknull
		jz done.prt
		out p3
		incr
		jp loop.prt
done.prt:
	ret
	
setfloatstr:
	cdr
	st floatstr::8
	out p0
	std floatstr::0
	out p1
ret

checknull:
	cdr
	ld r3
	in p2
	bt r3
ret

floatstr:
	.times 13, 0
	