 JP __main

A:
 DW 256
B:
 DW 257

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
 XOR R0
 LD R0
 POPD
 POP R1
 XOR R1
 PUSHD
 STL R0
 POPD
 OR R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 LD R0
 JZ endif_0
 ED
 STD 0x01
endif_0:
 ED
 STD 0x02
 JP __end

__end:


The file 'types.hex' was compiled successfully with 218 bytes!
