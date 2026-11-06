 JP __main

A:
 DW 255
B:
 DW 256
X:
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
 JC @+5
 CDR
 JP @+6
 JZ @-3
 STD 1
 LD R0
 POPD
 POP R1
 BT R1
 JZ @+7
 JC @+8
 CDR
 JP @+3
 STL R0
 LD R0
 CDR
 PUSHD
 STL R0
 PUSHD
 STD X::8
 OUT P0
 STD X::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
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
 JZ endif_0
 ED
 STD 0x01
endif_0:
 ED
 STD 0x02
 JP __end

__end:


The file 'types.hex' was compiled successfully with 342 bytes!
