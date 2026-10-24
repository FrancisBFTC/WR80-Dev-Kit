 JP __main

A:
 DB 0
B:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0x123
 LD R0
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 IN P2
 ADD R0
 PUSHD
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 POPD
 OUT P2
 STD 0x123::8
 PUSHD
 STD 0x123
 LD R0
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
 ADD R0
 JC @+12
 LD R0
 POPD
 POP R1
 ADD R1
 PUSHD
 STL R0
 JP @+12
 LD R0
 STD 0x80
 IDC
 POPD
 INCR
 POP R1
 ADD R1
 PUSHD
 STL R0
 PUSHD
 STD B::8
 OUT P0
 STD B::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 JP __end

__end:


The file 'test1.hex' was compiled successfully with 283 bytes!
