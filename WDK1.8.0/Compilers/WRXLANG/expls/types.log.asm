 JP __main

A:
 DW 1000
B:
 DW 10
Q:
 DW 0
R:
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
 PUSHD
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
 LD R1
 POP R2
 POP R3
 STL R1
 DIV R3
 PUSHD
 STD Q::8
 OUT P0
 STD Q::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
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
 PUSHD
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
 LD R1
 POP R2
 POP R3
 STL R1
 DIV R3
 PUSHD
 STD R::8
 OUT P0
 STD R::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 ED
 JP __end

__end:


The file 'types.hex' was compiled successfully with 377 bytes!
