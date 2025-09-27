db "WLL",0

SymbolTable:
	dw sum16_str, sum16

sum16_str:
	db "sum16",0

define PARAM1 4
define PARAM2 5
define PARAM3 6
define PARAM4 7

; Sum16 - Add two 16-bit values
; In: 
;	BP + 4 = High Part value  1
;	BP + 5 = Low Part value 1
;	BP + 6 = High Part value 2
;	BP + 7 = Low Part value 2
; Out:
;	R0:DR = 16-bit result
sum16:
	pushb
	pushs
	popb

	cdr
	st PARAM1
	abp
	ld r0
	
	cdr
	st PARAM2
	abp
	ld r1
	
	cdr
	st PARAM3
	abp
	ld r2
	
	cdr
	st PARAM2
	abp
	ld r3
	
	cdr
	or r0
	add r2
	ld r0
	
	cdr
	or r1
	add r3
	ld r1
	jc sum_high
	jp sum.end
	
sum_high:
	cdr
	st 1
	add r0
	ld r0
	
sum.end:
	push r1
	popd
	
	popb
ret


