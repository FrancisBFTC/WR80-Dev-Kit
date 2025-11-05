include "../../libraries/wr80asm/wr80x.asm"

define .SPACING 4
define .HIGH 	4
define .LOW 	0

macro .shift 1
	if #1 == r6
		.shl #1, 1
	endf
	else
		.shr #1, 1
	ende
endm

macro .nibble 1
_loop##:
	.shift #1
	.dec _r5
	.cmp r5, .NULL
	.jne _loop##
endm

macro .getnibble 1
	.pop 4
	.mov r5, DR
	.cmp r5, .NULL
	.je _skip##
	.nibble #1
_skip##:
endm

.outd data
.clear r3
.mov r0, 8

loop:
	.inb r1, p2
	.Invoke parse, .HIGH
	.Invoke parse, .LOW
	.inc _r3
	.inc _ram
	.mov r4, r3
	.mod r4, .SPACING
	.cmp r4, .NULL
	.jne check
	.mov r4, 32
	.outb p3, r4
check:
	.cmp r3, r0
	.jne loop
.END
	
parse:
	.mov BP, SP
	
	.mov r6, 15
	
	.getnibble r6

	.mov r2, r1
	.and r2, r6
	
	.getnibble r2

	.sub r2, 9
	.add r2, 64
	.outb p3, r2
	
	.mov SP, BP
.ret

data:
	.times 2, $BE, $BA, $CA, $FE
