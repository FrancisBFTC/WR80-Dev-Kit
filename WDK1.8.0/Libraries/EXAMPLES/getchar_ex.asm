jp main
	
include "../SYS8/Asm/stdio.inc"

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
