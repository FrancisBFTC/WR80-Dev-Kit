 JP __main

A:
 DW 0
B:
 DW 0
D:
 DW 0
E:
 DW 0
F:
 DW 0
G:
 DW 0
T1:
 DW 0
T2:
 DW 0
T3:
 DW 0
T4:
 DW 0
T5:
 DW 0
T6:
 DW 0
T7:
 DW 0
T8:
 DW 0
T9:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0x01
 PUSHD
 STD 0x02
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
 STD 0x03
 PUSHD
 STD 0x04
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
 STD 0x00
 PUSHD
 STD 0x02
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
 STD 0x00
 PUSHD
 STD 0x03
 PUSHD
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD 0x00
 PUSHD
 STD 0x04
 PUSHD
 STD F::8
 OUT P0
 STD F::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD 0x00
 PUSHD
 STD 0x05
 PUSHD
 STD G::8
 OUT P0
 STD G::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD G::8
 OUT P0
 STD G::0
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
 STD F::8
 OUT P0
 STD F::0
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
 LD R0
 PUSH R0
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
 LD R1
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
 LD R2
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
 LD R3
 MUL R2
 LD R4
 STL R0
 LD R5
 POPD
 MUL R2
 ADD R5
 LD R5
 POPD
 MUL R3
 ADD R5
 PUSHD
 STL R4
 POP R5
 POP R0
 PUSH R5
 ADD R1
 JC @+10
 LD R1
 POPD
 POP R2
 ADD R2
 PUSHD
 STL R1
 JP @+12
 LD R1
 STD 0x80
 IDC
 POPD
 INCR
 POP R2
 ADD R2
 PUSHD
 STL R1
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
 STD T1::8
 OUT P0
 STD T1::0
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
 STD T2::8
 OUT P0
 STD T2::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R0
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
 ADD R1
 JC @+10
 LD R1
 POPD
 POP R2
 ADD R2
 PUSHD
 STL R1
 JP @+12
 LD R1
 STD 0x80
 IDC
 POPD
 INCR
 POP R2
 ADD R2
 PUSHD
 STL R1
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
 STD T3::8
 OUT P0
 STD T3::0
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
 SUB R0
 JC @+4
 JP @+10
 LD R0
 POPD
 POP R1
 SUB R1
 PUSHD
 STL R0
 JP @+12
 LD R0
 STD 0x80
 IDC
 POPD
 DECR
 POP R1
 SUB R1
 PUSHD
 STL R0
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
 STD T4::8
 OUT P0
 STD T4::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R0
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
 LD R0
 PUSHD
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
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 STD T5::8
 OUT P0
 STD T5::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD G::8
 OUT P0
 STD G::0
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
 STD F::8
 OUT P0
 STD F::0
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
 LD R0
 PUSH R0
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 LD R0
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
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 STD T6::8
 OUT P0
 STD T6::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD G::8
 OUT P0
 STD G::0
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
 STD F::8
 OUT P0
 STD F::0
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
 LD R0
 PUSH R0
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 LD R0
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
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
 SUB R0
 JC @+4
 JP @+10
 LD R0
 POPD
 POP R1
 SUB R1
 PUSHD
 STL R0
 JP @+12
 LD R0
 STD 0x80
 IDC
 POPD
 DECR
 POP R1
 SUB R1
 PUSHD
 STL R0
 PUSHD
 STD T7::8
 OUT P0
 STD T7::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD F::8
 OUT P0
 STD F::0
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
 PUSH R0
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
 LD R1
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
 LD R2
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
 LD R3
 MUL R2
 LD R4
 STL R0
 LD R5
 POPD
 MUL R2
 ADD R5
 LD R5
 POPD
 MUL R3
 ADD R5
 PUSHD
 STL R4
 POP R5
 POP R0
 PUSH R5
 ADD R1
 JC @+10
 LD R1
 POPD
 POP R2
 ADD R2
 PUSHD
 STL R1
 JP @+12
 LD R1
 STD 0x80
 IDC
 POPD
 INCR
 POP R2
 ADD R2
 PUSHD
 STL R1
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
 STD T8::8
 OUT P0
 STD T8::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 STD F::8
 OUT P0
 STD F::0
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
 PUSH R0
 STD E::8
 OUT P0
 STD E::0
 OUT P1
 STD 0x01
 IDC
 INCR
 IN P2
 PUSHD
 DECR
 IN P2
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 LD R0
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
 LD R1
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
 LD R2
 MUL R1
 LD R3
 STL R0
 LD R4
 POPD
 MUL R1
 ADD R4
 LD R4
 POPD
 MUL R2
 ADD R4
 PUSHD
 STL R3
 POP R4
 POP R0
 PUSH R4
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
 STD T9::8
 OUT P0
 STD T9::0
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


The file 'types.hex' was compiled successfully with 4860 bytes!
