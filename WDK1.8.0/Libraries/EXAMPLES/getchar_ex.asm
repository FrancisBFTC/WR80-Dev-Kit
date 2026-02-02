jp main

;include "../SYS8/Asm/stdio.inc"

import "../SYS8/WLL/stdio.wll"
   getchar
   putchar
endx

main:
	clr
	
loop:
	call getchar
	pushd
	call putchar
	pushb
	pops
	jp loop
	
end:
	jp end
