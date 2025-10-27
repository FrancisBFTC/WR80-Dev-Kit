define STACK 0x40

macro .data name1, name2, num1, num2
	define #name1 #num1
	define #name2 #num2
	st 0
	dw #name1, #name2
endm

macro .code ...
	Main:
		std #1
		ssp
endm

.data ARG1, ARG2, STACK, $fe01
	; Data Section

.code STACK
	; Code Section with STACK

