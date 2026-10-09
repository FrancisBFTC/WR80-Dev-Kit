 JP __main


__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0xCA
 OUT P0
 STD 0xFE
 OUT P1
 ED
 JP __end

__end:


The file 'prog.hex' was compiled successfully with 57 bytes!
