 JP __main

X:
 DW 57346
Y:
 DW 53250

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD Y::8
 OUT P0
 STD Y::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R0
 STD X::8
 OUT P0
 STD X::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ @+12
 POPD
 POP R1
 BT R1
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_0
 ED
 STD 0xFF
 JP endif_1
else_0:
 ED
 STD 0x00
endif_1:
 JP __end

__end:


The file 'types.hex' was compiled successfully with 233 bytes!
