 JP __main

A:
 DW 65535
B:
 DW 65535
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
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ @+14
 POPD
 POP R1
 PUSH R1
 PUSH R1
 BT R1
 JZ @+5
 CDR
 JP @+4
 STD 1
 POP R1
 POP R1
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


The file 'op.hex' was compiled successfully with 386 bytes!
