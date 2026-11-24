 JP __main

numeros:
 DB 10, 20, 30, 40
expressoes:
 DB 3, 20, 25
palavras:
 DW 0x0064, 0x00C8, 0x012C, 0x0000, 0x0000

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 JP __end

__end:

