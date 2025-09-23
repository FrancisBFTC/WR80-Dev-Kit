define FILECOUNT 5

org 0x146

Start:
	call setinfo
	call print
	
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
	call getcmdsize
	pushd 				; salva quantidade de comandos
	
begin:
	call getregs
	call setaddr
	in p2
	pushd
	
	std 1
	call getoffset
	pushd
	
	std 1
	call incregs
	call saveregs
	
	pop r0
	pop r1
	
	call configbuff
	call strcmp
	jc checkcmd
	jp cmdfound
	
checkcmd:
	std 1
	ld r1
	popd
	sub r1
	jz checkbin
	pushd
	jp begin
	
checkbin:
	call copyname
	std FILECOUNT
	call configwrofs
	call breakline
	
	pop r2
	call 0x000
	jc error
	
	call breakline
	jp shell
	
cmdfound:
	popd
	; R0:R1 aponta para o zero (NULL) da string do comando
	; em commands. Somar +1 pra pegar o endereco de cmd.X
	
	; P0:P1 aponta para o zero (NULL) da string da CLI
	; em clibuff. Somar +1 pra pegar o argumento (em cmd.X)
	
	std 1
	call incregs 	; incrementa R0:R1
	
	push r0			; salva R0:R1 na pilha
	push r1
	call getaddr 	; pega P0:P1 do comando
	std 1
	call incregs	; incrementa R0:R1 (P0:P1)
	call saveregs	; salva P0:P1 em R2:R3
	pop r1
	pop r0
	
	push r2
	push r3
	call setaddr 	; configura P0:P1 com R0:R1
	in p2			; pega byte baixo
	pushd 			; salva byte baixo
	std 1
	call getoffset
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
	
error:
	call printerr
	jp shell
	
errorstr:
	db "error: command not found!",10,13,0
readstr:
	db "error: file not found!",0
info:
	db "--- WROS Shell for WR80X v1.5.2 ---",13,10
	db "read the 'XHELP.txt' file to see commands. :D",13,10,13,10,0
	
cmd.evt:
	dw 0x0000
	
cmd.vec:
	dw str.write, str.lf, str.read
	dw str.clear, str.exit
	
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
	str.clear:
		db "clear",0
		dw cmd.clear
	str.exit:
		db "exit",0
		dw cmd.exit
		
hexarg:
	db "-h",0
	
cmd.write:
	call print
	call sub8
	jz done.write
	std 0x20
	call writechar
	jp cmd.write
done.write:
	ret

cmd.lf:
	call configtable
	push r0
	push r1
	std FILECOUNT
	ld r2
	lf.loop:
		call print
		call sub8
		pop r1
		pop r0
		jz done.lf
		call printsize
		std 14
		call getoffset
		call breakline
		push r0
		push r1
		jp lf.loop
done.lf:
	call printsize
	ret
	
cmd.read:
	call getaddr
	push r0
	push r1
	cdr
	pushd
	st 2
	bt r2
	jz readarg
	jp noarg
readarg:
	call getarg
	call sethexarg
	call strcmp
	jc noarg
	popd
	std 1
	pushd
noarg:
	call configtable
	call saveregs
	popd
	pop r1
	pop r0
	push r0
	push r1
	pushd
	; R0:R1 = nome do arquivo na CLI
	; P0:P1 = nome do arquivo no WROFS (R2:R3)
	std FILECOUNT
	pushd
	read.loop:
		call strcmp
		jc read.cnt
		; arquivo encontrado
		popd
		popd
		pop r1
		pop r0
		pushd
		call getregs
		push r0
		push r1
		std 12
		call getoffset
		pushd
		std 1
		call getoffset
		ld r2
		popd
		ld r3
		
		pop r1
		pop r0
		std 10
		call getoffset
		pushd
		std 1
		call getoffset
		ld r0
		popd
		ld r1
		call setaddr
		call setincone
		pop r0
		
	data.loop:
		std 1
		bt r0
		in p2
		jz data.hex
		push r0
		call getbreak
		pop r0
		jz isbreak
		out p3
	data.incr:
		call incaddr
		call decregs
		jc ispositive
		jp data.loop
	ispositive:
		jz iszero
		jp data.loop
	iszero:
		stl r2
		jz read.done
		jp data.loop
	isbreak:
		call breakline
		jp data.incr
		
	data.hex:
		push r0
		push r1
		call printhex8
		pop r1
		pop r0
		std 0x20
		out p3
		jp data.incr
		
	read.cnt:
		call getregs
		std 14
		call getoffset
		call saveregs
		popd
		
		
		push r2
		pushd
		std 1
		ld r2
		popd
		sub r2
		pop r2
		jz read.error
		pop r4
		pop r1
		pop r0
		push r0
		push r1
		push r4
		pushd
		jp read.loop
		
read.error:
	popd
	popd
	popd
	cdr
	st readstr::8
	out p0
	std readstr::0
	out p1
	call print
	ret
read.done:
	ret
	
cmd.clear:
	clr
	clr
ret

cmd.exit:
	jp $FFF

include "kernel.inc"
