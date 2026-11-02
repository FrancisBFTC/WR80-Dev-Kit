 JP __main

A:
 DW 1000
B:
 DW 10
Q:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 ED
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
 POP R6
 STD 16
 LD R7
 STD 0x80
 LD R0
 PUSHD
 PUSH R6
 CDR
 LD R6
 PUSH R6
 PUSH R6
 STL R0
 AND R2
 JZ @+4
 STD 1
 LD R0
 POPD
 SHL 1
 OR R0
 LD R4
 JC @+5
 CDR
 JP @+4
 STD 1
 LD R0
 POPD
 SHL 1
 OR R0
 LD R5
 PUSH R2
 STL R5
 LD R2
 STD 20
 SBW
 STL R4
 LD R2
 STD 21
 SBW
 POP R2
 POP R0
 POPD
 PUSH R0
 SHR 1
 JZ @+4
 JP @+6
 STL R1
 LD R2
 STD 0x80
 LD R0
 POPD
 PUSH R0
 PUSHD
 PUSHD
 STL R3
 LD R6
 STL R5
 PUSHD
 STL R4
 SUB R3
 JC @+4
 JP @+10
 LD R3
 POPD
 POP R4
 SUB R4
 PUSHD
 STL R3
 JP @+21
 LD R3
 STD 0x80
 IDC
 POPD
 DECR
 JC @+4
 JP @+6
 POP R4
 SUB R4
 JP @+5
 POP R4
 SUB R4
 DC
 PUSHD
 STL R3
 LD R4
 POP R5
 STL R6
 LD R3
 JC @+14
 STD 21
 SBP
 LD R4
 STD 20
 SBP
 LD R5
 CDR
 PUSHD
 JP @+5
 STD 1
 PUSHD
 STD 23
 SBP
 SHL 1
 LD R6
 POPD
 OR R6
 PUSH R2
 LD R2
 STD 23
 SBW
 POP R2
 JC @+5
 CDR
 JP @+4
 STD 1
 LD R6
 STD 22
 SBP
 SHL 1
 OR R6
 PUSH R2
 LD R2
 STD 22
 SBW
 POP R2
 PUSH R5
 PUSH R4
 ED
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-151
 STD -4
 SSP
 PUSH R5
 PUSH R4
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
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
 JP __end

__end:


The file 'types.hex' was compiled successfully with 768 bytes!
