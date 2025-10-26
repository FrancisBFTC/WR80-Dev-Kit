define STACK 0x40

macro .data
	st 0
	db 0
endm

macro .code ...
	Main:
		std #1
		ssp
endm

.data
	; Data Section

.code STACK
	; Code Section with STACK

