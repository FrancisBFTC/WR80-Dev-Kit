 JP __main

A:
 DB 255
B:
 DB 255
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
 IN P2
 JZ @+4
 STD 1
 LD R0
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 IN P2
 JZ @+4
 STD 1
 OR R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 LD R0
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
 POP R1
 OR R1
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


The file 'op.hex' was compiled successfully with 289 bytes!
