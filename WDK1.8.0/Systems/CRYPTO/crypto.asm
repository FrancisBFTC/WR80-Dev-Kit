; -------------------------------------------
; Algoritmo de Criptografia
; Evento: Campus Party Goiás 5
; Autor: Wenderson Anjos
; -------------------------------------------

jp Start

include "../../Libraries/WR80ASM/wr80x.asm"

import "../../Libraries/SYS8/WLL/stdio.wll", "../../Libraries/SYS8/WLL/string.wll"
	Print
	Scan
	strlen
endx

Start:
	std crypt.str1::8
	out p0
	std crypt.str1::0
	out p1
	call Print
	
	std crypt.msg::8
	out p0
	std crypt.msg::0
	out p1
	call Scan
	
	std crypt.str2::8
	out p0
	std crypt.str2::0
	out p1
	call Print
	
	std crypt.key::8
	out p0
	std crypt.key::0
	out p1
	call Scan
	
	call Crypto
	std encrypted::8
	out p0
	std encrypted::0
	out p1
	call Print 
	std crypt.msg::8
	out p0
	std crypt.msg::0
	out p1
	call Print
	std $0A
	out p3
	
	call Crypto
	std decrypted::8
	out p0
	std decrypted::0
	out p1
	call Print 
	std crypt.msg::8
	out p0
	std crypt.msg::0
	out p1
	call Print
	std $0A
	out p3

.END
	
Crypto:
	std crypt.msg::8
	ld r0
	std crypt.msg::0
	ld r1
	
	std crypt.key::8
	ld r2
	std crypt.key::0
	ld r3
	
	.begin:
		push r1
		push r0
		call strlen
		pop r0
		pop r1
		
		ld r4
		push r3
		push r2
		call strlen
		pop r2
		pop r3
		
		ld r5
		cdr
		ld r7
		;ed
	.loop.crypto:
		stl r0
		out p0
		stl r1
		out p1
		in p2
		ld r6
		
		cdr
		bt r6
		jz .done.crypto
		
		std 0x01
		idc
		incr
		in p0
		ld r0
		in p1
		ld r1
		
		stl r2
		out p0
		stl r3
		out p1
		in p2
		
		xor r6
		xor r7
		pushd
		stl r0
		out p0
		stl r1
		out p1
		decr
		popd
		out p2
		incr
	
		std $7F
		idc
		incr
		
		push r0
		stl r7
		div r5
		stl r0
		ld r7
		pop r0
		
		stl r3
		add r7
		ld r3
		jp .loop.crypto
.done.crypto:
	stl r0
	out p0
	stl r1
	out p1
	cdr
	out p2
	ret

crypt.str1:
	db "Mensagem: ", 0
crypt.str2:
	db "Chave Secreta: ",0
	
crypt.msg:
	.times 64, 0
crypt.key:
	.times 64, 0
	
encrypted:
	db "Encriptado: ",0
decrypted:
	db "Decriptado: ",0
	