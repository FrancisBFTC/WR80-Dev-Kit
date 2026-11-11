 JP __main

W:
 DB 65

__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0x01
 JZ endif_0
 STD 2
 SSP
 STD W::8
 PUSHD
 STD W::0
 LD R2
 STD 3
 SBW
 POPD
 LD R2
 STD 2
 SBW
 STD 1
 SSP
 STD 2
 SBP
 PUSHD
 STD 3
 SBP
 OUT P1
 POPD
 OUT P0
 IN P2
 LD R2
 STD 4
 SBW
 STD 0x41
 LD R0
 STD 4
 SBP
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_1
 ED
 STD 0x01
 JP endif_2
else_1:
 ED
 STD 0x02
endif_2:
 STD -3
 SSP
endif_0:
 JP __end

__end:

