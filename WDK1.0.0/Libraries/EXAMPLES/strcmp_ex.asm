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
	
	cdr
	st Str2::8
	ld r2
	st Str2::4
	shl 4
	st Str2::0
	ld r3
	
	push r3
	push r2
	push r1
	push r0
	call strcmp
	
	; See DR in the WR80´s Bank Register
	; DR = 0 if equal, otherwise, DR != 0
	; use comparation and print here for checking too
	
end:
	jp end
	
Str1:
	db "dir",0

Str2:
	db "dir",0
	