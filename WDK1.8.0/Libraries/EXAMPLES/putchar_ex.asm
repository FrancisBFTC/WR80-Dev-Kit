jp main
	
;include "stdio.inc"

import "../SYS8/WLL/stdio.wll"
   putchar
endx
	
main:
	clr
	
	st 0x4
	shl 4
	st 0x1
	
	pushd	; Char 'A'
	call putchar
	
	st 0x6
	shl 4
	st 0x1
	
	pushd	; Char 'a'
	call putchar
	
	pushb
	pops
	
end:
	jp end
