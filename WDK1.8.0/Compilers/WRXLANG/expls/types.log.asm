 JP __main

var:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 CALL func
 LD R0
 PUSHB
 PUSHS
 POPB
 STD 4
 SBP
 POPB
 PUSHD
 STL R0
 LD R0
 CALL func
 LD R1
 PUSHB
 PUSHS
 POPB
 STD 4
 SBP
 POPB
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
 STD var::8
 OUT P0
 STD var::0
 OUT P1
 STD 0x01
 IDC
 POPD
 OUT P2
 INCR
 POPD
 OUT P2
 JP __end

func:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 2
 SSP
 STD 0xF02::8
 PUSHD
 STD 0xF02
 LD R2
 STD 3
 SBW
 POPD
 LD R2
 STD 2
 SBW
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 2
 SBP
 PUSHD
 STD 3
 SBP
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
 POP R2
 JP __func_end

__func_end:
 LD R0
 STD 2
 SBW
 STD -2
 SSP
 STL R0

 POP R0

 PUSHB
 POPS
 POPB
 RET

__end:


The file 'types.hex' was compiled successfully with 454 bytes!
