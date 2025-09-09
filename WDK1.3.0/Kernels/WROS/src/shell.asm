shell:
	call configcli
	
.loop:
	cdr
	ld r0
	call readkey
	
	call getbreak
	jz interpret
	
	call writechar
	jp .loop
	
interpret:
	cdr
	out p2
	
	call getcmdaddr
	call saveregs
	
begin:
	call getregs
	call setaddr
	in p2
	pushd
	
	call incregs
	call setaddr
	in p2
	pushd
	
	call incregs
	call saveregs
	
	pop r0
	pop r1
	
	call configbuff
	call strcmp
	jc begin
	
	
	jp .loop
	
cmd.vec:
	dw str.echo, str.ls, str.read
	
commands:
	str.echo:
		db "echo",0
		dw cmd.echo
	str.ls:
		db "ls",0
		dw cmd.ls
	str.read:
		db "read",0
		dw cmd.read
	
cmd.echo:

ret

cmd.ls:

ret

cmd.read:

ret

include "kernel.inc"
