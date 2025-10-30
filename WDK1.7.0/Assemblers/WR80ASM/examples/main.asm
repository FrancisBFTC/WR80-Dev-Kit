jp main

include "examples/math.inc"
include "examples/stdio.inc"
	
main:
	cdr
	st 1
	ld r0
	st 2
	ld r1
	st 3
	ld r2
	
	push r0
	push r1
	push r2
	call somar
	ld r0
	
	st String::4
	shl 4
	st String::0
	push r0
	pushd
	call printf
	
	pushb
	pops

end:
	jp end
	
String:
	db "Soma = ",0