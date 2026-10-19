 JP __main


__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 ED
 STD 0x00
 OUT P0
 STD 0x12
 OUT P1
 STD 0x0A
 OUT P2
 JP __end

__end:


The file 'prog.hex' was compiled successfully with 68 bytes!
