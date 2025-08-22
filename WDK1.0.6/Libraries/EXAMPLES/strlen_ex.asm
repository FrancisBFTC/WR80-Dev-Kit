jp main

include "string.inc"

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
	call strlen

	; See DR in bank register or print information
	
end:
	jp end
	
Str1:
	db "Hello World",0
