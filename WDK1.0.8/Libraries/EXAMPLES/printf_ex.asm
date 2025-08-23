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
	cdr
	
	st 9
	pushd
	push r1
	push r0
	call printf
	
	pushb
	pops
	
end:
	jp end
	
Str1:
	db "Number = ",0
