include "../../libraries/wr80asm/wr80x.asm"

define .SPACING 4
define .HIGH	4
define .LOW		0

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
	.push BP
	.push SP
	.pop BP
	.pop 4
	
	.add DR, 1
	.mov r5, DR
	.mov r6, 15
_loop1:
	.dec _r5
	.cmp r5, 0
	.je _end1
	.shl r6, 1
	.jmp _loop1
_end1:

	.mov r2, r1
	.and r2, r6
	.pop 4
	.add DR, 1
	.mov r5, DR
_loop2:
	.dec _r5
	.cmp r5, 0
	.je _end2
	.shr r2, 1
	.jmp _loop2	
_end2:

	.sub r2, 9
	.add r2, 64
	.outb p3, r2
	.push BP
	.pop SP
	.pop BP
.ret

data:
	.times 2, $BE, $BA, $CA, $FE
