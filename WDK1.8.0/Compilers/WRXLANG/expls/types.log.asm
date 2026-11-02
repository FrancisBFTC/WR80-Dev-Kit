 JP __main

A:
 DW 1000
B:
 DW 10
C:
 DW 2
X:
 DW 0
Y:
 DW 0
Z:
 DW 0
W:
 DW 0
K:
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STL R5
 LD R2
 STD 22
 SBW
 STL R4
 LD R2
 STD 23
 SBW
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
 PUSHD
 STD Y::8
 OUT P0
 STD Y::0
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
 LD R0
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
 ADD R0
 JC @+10
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
 STD Z::8
 OUT P0
 STD Z::0
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
 LD R0
 PUSHD
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
 LD R1
 POP R2
 POP R3
 STL R1
 MUL R3
 LD R4
 STL R0
 LD R5
 STL R2
 MUL R3
 ADD R5
 LD R5
 POPD
 MUL R1
 ADD R5
 PUSHD
 STL R4
 PUSHD
 STD W::8
 OUT P0
 STD W::0
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
 LD R0
 PUSHD
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
 LD R1
 POP R2
 POP R3
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
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
 STD 0x7F
 IDC
 DECR
 JZ @+4
 JP @-150
 STD -4
 SSP
 STD 22
 SBP
 PUSHD
 STD 23
 SBP
 PUSHD
 STD K::8
 OUT P0
 STD K::0
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


The file 'types.hex' was compiled successfully with 4427 bytes!
