 JP __main

A:
 DB 255
B:
 DB 255
C:
 DB 0

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
 LD R0
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ @+5
 CDR
 JP @+4
 STD 1
 LD R0
 PUSHD
 STD C::8
 OUT P0
 STD C::0
 OUT P1
 POPD
 OUT P2
 STD C::8
 OUT P0
 STD C::0
 OUT P1
 IN P2
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

The file 'op.hex' was compiled successfully with 224 bytes!