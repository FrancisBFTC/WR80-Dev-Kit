 JP __main

A:
 DW 1
var:
 DW 0

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0x01
 PUSHD
 STD 0x00
 PUSHD
 CALL func
 LD R0
 STD -2
 SSP
 PUSHB
 PUSHS
 POPB
 STD 6
 SBP
 POPB
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

 STD 2::8
 LD R2
 STD 2
 JP __func_end

__func_end:
 LD R0
 STD 2
 SBW
 STL R0

 POP R0

 POPB
 RET

__end:


The file 'types.hex' was compiled successfully with 221 bytes!
