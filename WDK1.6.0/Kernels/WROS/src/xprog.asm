ORG 1706

jp start

include "floatw8.asm"

start:
	call setstr
	call print
	
	call setfloatstr
	
	st 1
	ld r0
	std 64
	;shl 4
	ld r1
	push r1
	push r0
	call div8
	pop r1
	pop r1
	call EncodeFloat8
	;call PrintFloat8
	call ParseFloat8
	
	call setfloatstr
	call print
	
	pushb
	pops
	cdr
	st 2
	ssp
	
	dc
ret

str:
	db "Resultado : ",0
floatstr:
	db "            ",0

setfloatstr:
	cdr
	st floatstr::8
	out p0
	st floatstr::4
	shl 4
	st floatstr::0
	out p1
	cdr
ret

setstr:
	cdr
	st str::8
	out p0
	st str::4
	shl 4
	st str::0
	out p1
	cdr
ret

print:
	call setincone
	loop.prt:
		call checknull
		jz done.prt
		in p2
		out p3
		call incaddr
		jp loop.prt
done.prt:
	ret
	