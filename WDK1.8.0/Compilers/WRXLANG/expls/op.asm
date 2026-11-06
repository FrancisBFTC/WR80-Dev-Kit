 JP __main

A:
 DW 37008
B:
 DW 32928
C:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD B::8
 OUT P0
 STD B::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R0
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 BT R0
 JC @+6
 STD 1
 JP @+3
 CDR
 LD R0
 POPD
 POP R1
 BT R1
 JZ @+11
 JC @+6
 STD 1
 JP @+6
 CDR
 JP @+3
 STL R0
 LD R0
 CDR
 PUSHD
 STL R0
 PUSHD
 STD C::8
 OUT P0
 STD C::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD C::8
 OUT P0
 STD C::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 POP R0
 OR R0
 JZ else_0
 ED
 STD 0x01
 JP endif_1
else_0:
 ED
 STD 0x02
endif_1:
 JP __end

__end:


The file 'op.hex' was compiled successfully with 360 bytes!
