jp main
	
include "stdio.inc"
	
main:
	clr
	
	st Str1::8
	ld r0
	st Str1::4
	shl 4
	st Str1::0
	ld r1
	
	push r1
	push r0
	call puts
	
	pop r0
	pop r1
	
	push r1
	push r0
	call puts
	
	pushb
	pops
	
end:
	jp end
	
Str1:
	db "Hello World",0
