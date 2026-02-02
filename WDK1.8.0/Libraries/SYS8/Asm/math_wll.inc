define PARAM1 4
define PARAM2 5
define PARAM3 6
define PARAM4 7

export sum16
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
	st PARAM4
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
endx

export sub16
; Sub16 - Subtract two 16-bit values
; In: 
;	BP + 4 = High Part value  1
;	BP + 5 = Low Part value 1
;	BP + 6 = High Part value 2
;	BP + 7 = Low Part value 2
; Out:
;	R0:DR = 16-bit result
sub16:
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
	st PARAM4
	abp
	ld r3
	
	cdr
	or r0
	sub r2
	ld r0
	
	cdr
	or r1
	sub r3
	ld r1
	
	jc sub.end

	cdr
	st 1
	ld r2
	cdr
	or r0
	sub r2
	ld r0
	
sub.end:
	push r1
	popd
	
	popb
ret
endx

export mul8
; Mul8 - Multiply two 8-bit values
; In: 
;	BP + 4 = value 1
;	BP + 5 = value 2
; Out:
;	R0:DR = 16-bit result
mul8:
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
	st 1
	ld r2
	cdr
	ld r3
	
mul.begin:
	or r0
	bt r1
	jc skipxchg
	
	push r0
	push r1
	pop r0
	pop r1
	
skipxchg:
	cdr
	or r1
	bt r2
	jz ret.r0
	bt r3
	jz ret.zero
	
	cdr
	pushd
	or r0
	ld r3
	
mul.loop:
	cdr
	or r1
	sub r2
	ld r1
	jz ret.val
	cdr
	or r3
	add r0
	ld r3
	jc mul.high
	jp mul.loop
mul.high:
	popd
	add r2
	pushd
	jp mul.loop
	
ret.r0:
	push r0
	cdr
	ld r0
	popd
	jp mul8.ret
ret.zero:
	cdr
	ld r0
	jp mul8.ret
ret.val:
	pop r0
	push r3
	popd
mul8.ret:
	popb
ret
endx

export exp8
; Exp8 - Exponential of two 8-bit values
; In: 
;	BP + 4 = value 1 (base)
;	BP + 5 = value 2 (expoent)
; Out:
;	R0:DR = 16-bit result
exp8:
	pushb
	pushs
	popb
	
	cdr
	st 1
	ld r2
	cdr
	ld r3
	st PARAM1
	abp
	ld r0
	
	cdr
	st PARAM2
	abp
	
exp8.begin:
	bt r2
	jz exp8.r0
	bt r3
	jz exp8.ret1
	
	sub r2
	pushd
	cdr
	or r0
	ld r3
	push r2
	
exp8.loop:
	push r0
	push r3
	call mul8
	ld r3
	
	popd
	cdr
	or r0
	ld r1
	pop r0
	pop r2
	popd
	
	sub r2
	jz exp8.val
	pushd
	push r2
	jp exp8.loop
	
exp8.r0:
	push r0
	cdr
	ld r0
	popd
	jp exp8.ret
exp8.ret1:
	cdr
	ld r0
	st 1
	jp exp8.ret
exp8.val:
	push r1
	pop r0
	push r3
	popd
	
exp8.ret:
	popb
ret
endx

export div8
; Div8 - Divide two 8-bit values
; In: 
;	BP + 4 = value 1
;	BP + 5 = value 2
; Out:
;	R0:DR = 16-bit result
; 	R1, R2 and R3 preserveds.
div8:
	pushb
	pushs
	popb
	push r1
	
	cdr
	st PARAM1
	abp
	ld r0
	
	cdr
	st PARAM2
	abp
	ld r1
	
	push r2
	push r3
	
	cdr
	st 1
	ld r2
	cdr
	ld r3
	
div8.begin:
	cdr
	or r1
	bt r2
	jz div8.r0
	bt r3
	jz div8.zero
	cdr
	or r0
	bt r3
	jz div8.zero
	
div8.loop:
	pushd
	cdr
	or r3
	add r2
	ld r3
	popd
	sub r1
	ld r0
	jz div8.val
	jc div8.loop
	
	cdr
	or r0
	add r1
	ld r0
	cdr
	or r3
	sub r2
	ld r3
	jp div8.val
	
div8.r0:
	push r0
	cdr
	ld r0
	popd
	jp div8.ret
div8.zero:
	cdr
	ld r0
	jp div8.ret
div8.val:
	push r3
	popd
	
div8.ret:
	pop r3
	pop r2
	pop r1
	
	popb
ret
endx
