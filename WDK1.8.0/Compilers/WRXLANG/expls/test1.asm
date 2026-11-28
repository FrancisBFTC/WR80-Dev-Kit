 JP __main

funcs:
 DW func1, func2, func3
i:
 DB 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
while_begin_0:
 STD 0x01
 JZ while_end_1
 STD 0x03
 LD R0
 STD i::8
 OUT P0
 STD i::0
 OUT P1
 IN P2
 DIV R0
 STL R0
 LD R0
 CDR
 PUSHD
 STL R0
 SHL 1
 LD R0
 JC @+5
 CDR
 JP @+4
 STD 1
 LD R1
 POPD
 SHL 1
 OR R1
 PUSHD
 STL R0
 LD R0
 STD funcs::8
 PUSHD
 STD funcs::0
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
 STD 0x01
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

The file 'test1.hex' was compiled successfully with 498 bytes!