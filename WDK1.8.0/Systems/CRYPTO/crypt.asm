jp Main

include "../../Libraries/WR80ASM/wr80x.asm"

Main:
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
	
	std crypt.msg::8
	out p0
	std crypt.msg::0
	out p1
	call Print
	
	jp $FFF
	
.END

Print:
	cdr
	ld r0
	.print:
		in p2
		bt r0
		jz .done.print
		out p3
		std 0x01
		idc
		incr
		jp .print
.done.print:
	ret

Scan:
	cdr
	ld r0
	std $0D
	ld r1
	.scan:
		in p3
		bt r0
		jz .scan
		bt r1
		jz .done.scan
		out p3
		out p2
		std 0x01
		idc
		incr
		jp .scan
.done.scan:
	cdr
	out p2
	std $0A
	out p3
	ret

crypt.str1:
	db "Mensagem: ",0
	
crypt.str2:
	db "Chave: ",0
	
crypt.msg:
	.times 32, 0
	
crypt.key:
	.times 32, 0
