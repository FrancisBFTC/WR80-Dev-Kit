include "../../Libraries/WR80ASM/wr80x.asm"

.jmp Main

include "../../Libraries/SYS8/Asm/string.inc"

Main:
	.outd crypt.str1
	.Invoke Print
	.outd crypt.msg
	.Invoke Scan
	.outd crypt.str2
	.Invoke Print
	.outd crypt.key
	.Invoke Scan
	
	.Invoke Crypto
	.outd encrypted
	.Invoke Print
	.outd crypt.msg
	.Invoke Print
	.mov r0, 10
	.outb p3, r0
	
	.Invoke Crypto
	.outd decrypted
	.Invoke Print
	.outd crypt.msg
	.Invoke Print
	.mov r0, 10
	.outb p3, r0
.END

Print:
	;.inb r0, p2
	;.cmp r0, 0
	;.je .done
	;.outb p3, r0
	;.inc _ram
	;.jmp Print
.done:
	.ret
	
Scan:
	;.inb r0, p3
	;.cmp r0, 0
	;.je Scan
	;.cmp r1, 13
	;.je .scan.done
	;.outb p3, r0
	;.outb p2, r0
	;.inc _ram
	;.jmp Scan
.scan.done:
	;.mov r0, 0
	;.outb p2, r0
	;.mov r0, 10
	;.outb p3, r0
.ret
	
Crypto:
	
.ret

crypt.str1:
	db "Message: ", 0
crypt.str2:
	db "Secret Key: ",0
	
crypt.msg:
	.times 32, 0
crypt.key:
	.times 32, 0
	
encrypted:
	db "Encrypted: ",0
decrypted:
	db "Decrypted: ",0

