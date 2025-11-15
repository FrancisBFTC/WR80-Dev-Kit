define STACK 0x40

macro .data name1, name2, num1, num2
	define #name1 #num1
	define #name2 #num2
	st 0
	dw #name1, #name2
endm

macro .code ...
	ORG #3
	std #1
	ssp
	std #2
	.data #4, #5, #2, #3
endm

Main:
	.data ARG1, ARG2, STACK, $fe01

	.code STACK, 5, STACK, a0, a1
jp Main

