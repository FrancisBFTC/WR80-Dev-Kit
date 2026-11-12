
__main:
 PUSHB
 PUSHS
 POPB

 PUSH R0
 STD 0x01
 JZ endif_0
 ED
 STD 0x02
endif_0:
 JP __end

__end:

