jp main

Buffer:
	dw 0x0000

include "math.inc"

; Program to add two values:
; Buffer = sum16(0x1C0, 0x2C0) = 0x480
main:
	clr
	st 1
	ld r0
	st $0C
	shl 4
	ld r1
	cdr
	st 2
	ld r2
	st $0C
	shl 4
	ld r3
	
	push r3
	push r2
	push r1
	push r0
	call sum16
	
	pushb
	pops
	
	pushd
	push r0
	cdr
	st Buffer::0
	out p1
	
	popd
	out p2
	
	cdr
	st 1
	ld r1
	in p1
	add r1
	out p1
	
	popd
	out p2
	
end:
	jp end