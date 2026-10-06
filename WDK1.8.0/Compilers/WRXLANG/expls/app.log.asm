 JP __main

UNSIGNED:
 DB 0
SIGNED:
 DB 1
string:
 DB "ola pessoas",0

__main:
 PUSHB
 PUSHS
 POPB

 STD 0x000
 LD R0
 JP @+14
 DB "ola pessoas",0
 STD (@-12) >> 8
 PUSHD
 STD (@-15) & 0xFF
 PUSHD
 STD string::8
 PUSHD
 STD string::0
 PUSHD
 CALL compare
 LD R1
 STD -4
 SSP
 STL R1
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_32
 JP @+12
 DB "E igual\n",0
 STD (@-10) >> 8
 PUSHD
 STD (@-13) & 0xFF
 PUSHD
 CALL print
 LD R0
 STD -2
 SSP
 STL R0
 JP endif_33
else_32:
 JP @+16
 DB "E diferente\n",0
 STD (@-14) >> 8
 PUSHD
 STD (@-17) & 0xFF
 PUSHD
 CALL print
 LD R0
 STD -2
 SSP
 STL R0
endif_33:
 STD 0x000
 LD R0
 JP @+12
 DB "Oi pessoa",0
 STD (@-10) >> 8
 PUSHD
 STD (@-13) & 0xFF
 PUSHD
 STD string::8
 PUSHD
 STD string::0
 PUSHD
 CALL compare
 LD R1
 STD -4
 SSP
 STL R1
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_34
 JP @+12
 DB "E igual\n",0
 STD (@-10) >> 8
 PUSHD
 STD (@-13) & 0xFF
 PUSHD
 CALL print
 LD R0
 STD -2
 SSP
 STL R0
 JP endif_35
else_34:
 JP @+16
 DB "E diferente\n",0
 STD (@-14) >> 8
 PUSHD
 STD (@-17) & 0xFF
 PUSHD
 CALL print
 LD R0
 STD -2
 SSP
 STL R0
endif_35:
 STD string::8
 PUSHD
 STD string::0
 PUSHD
 CALL length
 LD R0
 STD -2
 SSP
 STL R0
 LD R0
 STD string::8
 PUSHD
 STD string::0
 PUSHD
 CALL length
 LD R1
 STD -2
 SSP
 STL R1
 ADD R0
 PUSHD
 JP @+16
 DB "Tamanho: %u\n",0
 STD (@-14) >> 8
 PUSHD
 STD (@-17) & 0xFF
 PUSHD
 CALL print
 LD R0
 STD -3
 SSP
 STL R0
 JP __end

print_char:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 4
 ABP
 OUT P3

__print_char_end:

 POP R0

 POPB
 RET

print_hex:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 1
 SSP
 STD 1
 SSP
 STD 5
 ABP
 LD R2
 STD 3
 SBW
 STD 0x030
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 STD 0x078
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
while_begin_0:
 STD 3
 SBP
 JZ while_end_1
 STD 0x004
 LD R0
 STD 3
 SBP
 SUB R0
 LD R2
 STD 3
 SBW
 STD 0x00F
 LD R0
 STD 0x49
 IDC
 STD 4
 ABP
 PUSHD
 STD 3
 SBP
 JZ skip_shift_0
 LD R1
 DECR
 POPD
 SHR 1
 DECR
 JC @-2
 PUSHD
 skip_shift_0:
 POPD
 AND R0
 LD R2
 STD 2
 SBW
 STD 0x00A
 LD R0
 STD 2
 SBP
 BT R0
 JC @+6
 STD 1
 JP @+3
 CDR
 JZ else_2
 STD 0x030
 LD R0
 STD 2
 SBP
 ADD R0
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 JP endif_3
else_2:
 STD 0x041
 LD R0
 STD 0x00A
 LD R1
 STD 2
 SBP
 SUB R1
 ADD R0
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
endif_3:
 JP while_begin_0
while_end_1:

__print_hex_end:
 LD R0
 STD -2
 SSP
 STL R0

 POP R0

 PUSHB
 POPS
 POPB
 RET

print_num:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 1
 SSP
 STD 0x064
 LD R2
 STD 2
 SBW
 STD 5
 ABP
 JZ @+4
 STD 1
 LD R0
 STD 0x080
 LD R1
 STD 4
 ABP
 AND R1
 JZ @+4
 STD 1
 AND R0
 JZ endif_4
 STD 1
 LD R0
 STD 4
 ABP
 LD R1
 NOT R1
 ADD R0
 LD R2
 STD -4
 SBW
 STD 0x02D
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
endif_4:
while_begin_5:
 STD 0x000
 LD R0
 STD 2
 SBP
 BT R0
 JZ @+6
 STD 1
 JP @+3
 CDR
 JZ while_end_6
 STD 4
 ABP
 JZ @+5
 CDR
 JP @+4
 STD 1
 LD R0
 JZ @+4
 STD 1
 LD R0
 STD 2
 SBP
 LD R1
 STD 4
 ABP
 BT R1
 JC @+5
 CDR
 JP @+4
 STD 1
 JZ @+4
 STD 1
 OR R0
 JZ endif_7
 STD 0x030
 LD R0
 PUSH R0
 STD 2
 SBP
 LD R1
 STD 4
 ABP
 DIV R1
 POP R0
 ADD R0
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
endif_7:
 STD 2
 SBP
 LD R0
 STD 4
 ABP
 DIV R0
 STL R0
 LD R2
 STD -4
 SBW
 PUSH R0
 STD 0x00A
 LD R0
 STD 2
 SBP
 DIV R0
 POP R0
 LD R2
 STD 2
 SBW
 JP while_begin_5
while_end_6:

__print_num_end:
 LD R0
 STD -1
 SSP
 STL R0

 POP R0

 PUSHB
 POPS
 POPB
 RET

puts:
 PUSHB
 PUSHS
 POPB

 PUSH R0

while_begin_8:
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ while_end_9
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 JP while_begin_8
while_end_9:

__puts_end:

 POP R0

 PUSHB
 POPS
 POPB
 RET

print:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 1
 SSP
 STD 0x000
 LD R2
 STD 2
 SBW
while_begin_10:
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ while_end_11
 STD 0x025::8
 PUSHD
 STD 0x025
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+6
 STD 1
 JP @+3
 CDR
 JZ else_12
 STD 0x05C::8
 PUSHD
 STD 0x05C
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+6
 STD 1
 JP @+3
 CDR
 JZ else_14
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 JP endif_15
else_14:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 STD 0x06E::8
 PUSHD
 STD 0x06E
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_16
 STD 0x00A
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 JP endif_17
else_16:
 STD 0x074
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_18
 STD 0x009
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
 JP endif_19
else_18:
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
endif_19:
endif_17:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 JP while_begin_10
endif_15:
 JP endif_13
else_12:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 STD 1
 SSP
 STD 2
 SBP
 LD R0
 STD 6
 ADD R0
 ABP
 LD R2
 STD 3
 SBW
 STD 0x064
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_20
 STD SIGNED::8
 OUT P0
 STD SIGNED::0
 OUT P1
 IN P2
 PUSHD
 STD 3
 SBP
 PUSHD
 CALL print_num
 LD R0
 STD -2
 SSP
 STL R0
 JP endif_21
else_20:
 STD 0x075
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_22
 STD UNSIGNED::8
 OUT P0
 STD UNSIGNED::0
 OUT P1
 IN P2
 PUSHD
 STD 3
 SBP
 PUSHD
 CALL print_num
 LD R0
 STD -2
 SSP
 STL R0
 JP endif_23
else_22:
 STD 0x078
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ else_24
 STD 0x008
 PUSHD
 STD 3
 SBP
 PUSHD
 CALL print_hex
 LD R0
 STD -2
 SSP
 STL R0
 JP endif_25
else_24:
 STD 0x061
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+5
 CDR
 JP @+4
 STD 1
 JZ endif_26
 STD 3
 SBP
 PUSHD
 CALL print_char
 LD R0
 STD -1
 SSP
 STL R0
endif_26:
endif_25:
endif_23:
endif_21:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 STD 0x001
 LD R0
 STD 2
 SBP
 ADD R0
 LD R2
 STD 2
 SBW
 JP while_begin_10
 STD -1
 SSP
endif_13:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 JP while_begin_10
while_end_11:

__print_end:
 LD R0
 STD -1
 SSP
 STL R0

 POP R0

 PUSHB
 POPS
 POPB
 RET

length:
 PUSHB
 PUSHS
 POPB

 PUSH R0

 STD 2
 SSP
 STD 0x000::8
 PUSHD
 STD 0x000
 LD R2
 STD 3
 SBW
 POPD
 LD R2
 STD 2
 SBW
while_begin_27:
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ while_end_28
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
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 JP while_begin_27
while_end_28:
 STD 3
 SBP
 JP __length_end

__length_end:
 LD R0
 STD -2
 SSP
 STL R0

 POP R0

 PUSHB
 POPS
 POPB
 RET

compare:
 PUSHB
 PUSHS
 POPB

 PUSH R0

while_begin_29:
 STD 7
 ABP
 PUSHD
 STD 6
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ @+4
 STD 1
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ @+4
 STD 1
 OR R0
 JZ while_end_30
 STD 7
 ABP
 PUSHD
 STD 6
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 BT R0
 JZ @+6
 STD 1
 JP @+3
 CDR
 JZ endif_31
 STD 1
 JP __compare_end
endif_31:
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
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
 STD -4
 SBW
 POPD
 LD R2
 STD -5
 SBW
 STD 0x001::8
 PUSHD
 STD 0x001
 LD R0
 STD 7
 ABP
 PUSHD
 STD 6
 ABP
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
 STD -6
 SBW
 POPD
 LD R2
 STD -7
 SBW
 JP while_begin_29
while_end_30:
 STD 7
 ABP
 PUSHD
 STD 6
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ @+4
 STD 1
 LD R0
 STD 5
 ABP
 PUSHD
 STD 4
 ABP
 OUT P1
 POPD
 OUT P0
 IN P2
 JZ @+4
 STD 1
 OR R0
 JP __compare_end

__compare_end:

 POP R0

 PUSHB
 POPS
 POPB
 RET

__end:


The file 'app.hex' was compiled successfully with 5189 bytes!
