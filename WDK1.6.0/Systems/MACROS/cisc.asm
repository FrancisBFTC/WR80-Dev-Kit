include "../../libraries/wr80asm/wr80x.asm"
define .SPACING 4	; espaçamento entre palavras

.outd data
.clear r3
.movl r0, 8

loop:
	.inb r1, p2
	.Invoke parse
	.inc _r3
	.inc _ram
	.mov r4, r3
	.mod r4, .SPACING
	.cmpl r4, 0
	.je space
	.jmp check

space:
	.movl r4, 0x20
	.outb p3, r4
check:
	.cmp r3, r0
	.jne loop
.END
	
parse:
	.mov r2, r1
	.andl r2, $F0
	.shr r2, 4
	.subl r2, 0x09
	.addl r2, 0x40
	.outb p3, r2
	.mov r2, r1
	.andl r2, $0F
	.subl r2, 0x09
	.addl r2, 0x40
	.outb p3, r2
ret

data:
	.times 2, $BE, $BA, $CA, $FE
