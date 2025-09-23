include "boot.asm"

FileTable:
	Entry1:
		db "boot.bin",0,0
		dw Boot
		dw 512
	
	Entry2:
		db "shell.bin",0
		dw shell
		dw 57
		
include "shell.asm"

ORG 0xFFF
db 0
