org 0x11C

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
	call formatcli
	push r2				; Salva contagem de argumentos/espaços
	call getcmdaddr
	call saveregs
	
begin:
	call getregs
	call setaddr
	in p2
	pushd
	
	cdr
	st 1
	call incregs
	call setaddr
	in p2
	pushd
	
	cdr
	st 1
	call incregs
	call saveregs
	
	pop r0
	pop r1
	
	call configbuff
	call strcmp
	jc begin
	
	; R0:R1 aponta para o zero (NULL) da string do comando
	; em commands. Somar +1 pra pegar o endereco de cmd.X
	
	; P0:P1 aponta para o zero (NULL) da string da CLI
	; em clibuff. Somar +1 pra pegar o argumento (em cmd.X)
	
	cdr
	st 1
	call incregs 	; incrementa R0:R1
	
	push r0			; salva R0:R1 na pilha
	push r1
	call getaddr 	; pega P0:P1 do comando
	cdr
	st 1
	call incregs	; incrementa R0:R1 (P0:P1)
	call saveregs	; salva P0:P1 em R2:R3
	pop r1
	pop r0
	
	push r2
	push r3
	call setaddr 	; configura P0:P1 com R0:R1
	in p2			; pega byte baixo
	pushd 			; salva byte baixo
	cdr
	st 1
	call incregs 	; incrementa R0:R1
	call setaddr 	; configura P0:P1
	in p2			; pega byte alto
	pushd			; salva byte alto
	call setcmdevt	; P0:P1 = R0:R1 = cmd.evt 
	call configevt
	popd
	popd
	pop r1
	pop r0
	
	pop r2			; Restaura contagem de argumentos/espaços
	call breakline
	call setaddr
	call cmd.evt
	call breakline
	
	jp shell
	
cmd.evt:
	dw 0x0000
	
cmd.vec:
	dw str.write, str.lf, str.read
	
commands:
	str.write:
		db "write",0
		dw cmd.write
	str.lf:
		db "lf",0
		dw cmd.lf
	str.read:
		db "read",0
		dw cmd.read
	
cmd.write:
	call print
	call sub8
	jz done.write
	st 2
	shl 4
	call writechar
	jp cmd.write
done.write:
	ret

cmd.lf:
	call configtable
	push r0
	push r1
	cdr
	st 2
	ld r2
	lf.loop:
		call print
		call sub8
		pop r1
		pop r0
		jz done.lf
		call printsize
		cdr
		st 14
		call incregs
		call setaddr
		call breakline
		push r0
		push r1
		jp lf.loop
done.lf:
	call printsize
	ret
	
cmd.read:
	cdr
	st 0x4
	shl 4
	st 0x3
	out p3
ret

include "kernel.inc"
