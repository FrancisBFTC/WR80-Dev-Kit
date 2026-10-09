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
 DB 0

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
 POPD
 OUT P2
while_begin_0:
 STD 0x001
 JZ while_end_1
 STD 2
 SSP
 STD 0x006::8
 PUSHD
 STD 0x006
 LD R0
 STD i::8
 OUT P0
 STD i::0
 OUT P1
 IN P2
 DIV R0
 STL R0
 LD R0
 STD A::8
 PUSHD
 STD A::0
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
 LD R2
 STD 3
 SBW
 POPD
 LD R2
 STD 2
 SBW
 STD 2
 SBP
 PUSHD
 STD 3
 SBP
 OUT P1
 POPD
 OUT P0
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
 OUT P1
 POPD
 OUT P0
 IN P2
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
 STD 0x002
 LD R0
 STD i::8
 OUT P0
 STD i::0
 OUT P1
 IN P2
 ADD R0
 PUSHD
 STD i::8
 OUT P0
 STD i::0
 OUT P1
 POPD
 OUT P2
 STD -2
 SSP
 JP while_begin_0
while_end_1:
 JP __end

func1:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 0x041
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

 STD 0x042
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

 STD 0x043
 OUT P3

__func3_end:

 POP R0

 POPB
 RET

__end:


The file 'test.hex' was compiled successfully with 733 bytes!
