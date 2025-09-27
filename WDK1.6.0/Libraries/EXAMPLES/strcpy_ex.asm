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
	st Buf::8
	ld r2
	st Buf::4
	shl 4
	st Buf::0
	ld r3
	
	; ---
	push r3
	push r2
	push r1
	push r0
	call strcpy
	; ---
	
	; Uncomment below for strncpy call and comment above between the '---' line
	; cdr
	; st 3
	; push r3
	; push r2
	; push r1
	; push r0
	; pushd
	; call strncpy
	
	; See R0 and DR in the bank register
	; See also the logisim hexa editor in 0xB2 address
	
end:
	jp end
	
Str1:
	db "string copied",0
Buf:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
