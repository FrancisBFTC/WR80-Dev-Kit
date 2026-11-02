 JP __main

A:
 DW 0
B:
 DW 0
C:
 DW 0
X:
 DW 0
i:
 DW 0
D:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD func1::8
 PUSHD
 STD func1::0
 PUSHD
 STD A::8
 OUT P0
 STD A::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD func2::8
 PUSHD
 STD func2::0
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
 STD func3::8
 PUSHD
 STD func3::0
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
while_begin_0:
 STD 0x01
 JZ while_end_1
 STD 0x00
 PUSHD
 STD 0x06
 LD R0
 PUSHD
 STD i::8
 OUT P0
 STD i::0
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
 STD 23
 SBP
 PUSHD
 STD 22
 SBP
 LD R0
 STD A::8
 PUSHD
 STD A::0
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
 STD D::8
 OUT P0
 STD D::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD D::8
 OUT P0
 STD D::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 OUT P1
 POPD
 OUT P0
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
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
 PUSHD
 POP R1
 POP R2
 STD 0x51
 IDC
 DECR
 STD (@+8) >> 8
 PUSHD
 STD (@+5) & 0xFF
 PUSHD
 PUSH R2
 PUSH R1
 RET
 STD 0x00
 PUSHD
 STD 0x02
 LD R0
 STD i::8
 OUT P0
 STD i::0
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
 STD i::8
 OUT P0
 STD i::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 JP while_begin_0
while_end_1:
 JP __end

func1:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 0x41
 OUT P3

__func1_end:

 POP R0

 POPB
 RET

func2:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 0x42
 OUT P3

__func2_end:

 POP R0

 POPB
 RET

func3:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 0x43
 OUT P3

__func3_end:

 POP R0

 POPB
 RET

__end:


The file 'test.hex' was compiled successfully with 1551 bytes!
