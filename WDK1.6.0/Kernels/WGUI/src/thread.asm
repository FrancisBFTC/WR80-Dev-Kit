StartTasks:
	clr
	call ConfigTimer
	call ConfigKeyboard
	
	std IntTable::8
	out p0
	std IntTable::0
	out p1
	ei
	
	std 0x34
	ld r0
	std 0x61
	ld r4
	std 0x62
	ld r5
	std 0x63
	ld r6
	pushs
	popb
	std 0x20
	ssp
	
.LockMainThread:
	call ReadKey
	bt r0
	jz .StartTasks.ret
	bt r4
	jz .CreateTask1
	bt r5
	jz .CreateTask2
	bt r6
	jz .CreateTask3
	jp .LockMainThread

.CreateTask1:
	di
	push r0
	call DefProc1
	jp .CreateTask
	
.CreateTask2:
	di
	push r0
	call DefProc2
	jp .CreateTask
	
.CreateTask3:
	di
	push r0
	call DefProc3
	
.CreateTask:
	call ClearKey
	call CreateProcess
	jc .TaskError
	pop r0
	ei
	jp .LockMainThread
	
.TaskError:
	std _taskerror::8
	out p0
	std _taskerror::0
	out p1
	call Print
	pop r0
	ei
	jp .LockMainThread
	
.StartTasks.ret:
	pushb
	pops
	ret

DefProc1:
	std Process1::8
	ld r0
	std Process1::0
	ld r1
ret

DefProc2:
	std Process2::8
	ld r0
	std Process2::0
	ld r1
ret

DefProc3:
	std Process3::8
	ld r0
	std Process3::0
	ld r1
ret

	
IntTable:
	dw Keyboard
	dw Mouse
	dw Timer
	dw Unknown
	
define PID_1 0x0001
define PID_2 0x0002
define PID_3 0x0003
define PID_END 0x0000
define TABLE_SIZE 4
	
ProcTable:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,   PID_END
	
ProcIndex:
	db 0

	
; ISR 0 ------------
Keyboard:
	di
	pushd
	push r0
	in p0
	pushd
	in p1
	pushd
	
	std $FA
	ld r0
	std 0x02
	out p7
	call WaitACK
	std 0x00
	out p7
	call WaitACK
	in p7
	out p3
	ld r0
	std key_buff::8
	out p0
	std key_buff::0
	out p1
	stl r0
	out p2
	
	popd
	out p1
	popd
	out p0
	pop r0
	popd
	ei
iret
key_buff:
	db 0
; ------------------

; ISR 1 ------------
Mouse:

iret
; ------------------

; ISR 2 ------------
Timer:
	di
	pushs
	pushb
	pushd
	in p0
	pushd
	in p1
	pushd
	in p4
	pushd
	in p5
	pushd
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	
	cdr
	ld r3
	
	call calc_table
	
	std 16
	ld r0
	std 1
	ld r1
	std 0x01
	idc
.loop_context:
	popd
	out p2
	incr
	stl r0
	sub r1
	ld r0
	jc .loop_context
	
	pushs
	popb
	cdr
	abp
	out p2
	incr
	std 1
	abp
	out p2
	incr
	
.NextProc:
	call read_proc
	
	std PID_END::0
	bt r4
	jz .RestoreMainThread
	cdr
	bt r7
	jz .ProcessBegin	; Se status = 0, Inicia processo
	
	incr
	std 0x03
	bt r7
	jz .SumToNextProc
	std .ProcessCall::8
	pushd
	std .ProcessCall::0
	pushd
	jp .RestoreContext
	
.SumToNextProc:
	cdr
	ld r2
	in p0
	ld r0
	in p1
	ld r1
	std 19
	call sum_address
	jp .NextProc
	
.RestoreMainThread:
	call set_proc_index
	cdr
	out p2
	call set_proc_table
	
	jp .RestoreContext
	
.ProcessBegin:
	std 0x01
	out p2
	
	std 0x50
	ssp
	
	std .ProcessCall::8
	pushd
	std .ProcessCall::0
	pushd
	std 0x41
	idc
	decr
	push r0
	push r1
	clr
	
.ProcessCall:
	ret
	
	call calc_table
	
	decr
	std 0x03
	out p2
	
	incr
	jp .SumToNextProc
	
.RestoreContext:
	
	std 0x01
	idc
	in p2
	ld r7
	incr
	in p2
	ld r6
	incr
	in p2
	ld r5
	incr
	in p2
	ld r4
	incr
	in p2
	ld r3
	incr
	in p2
	ld r2
	incr
	in p2
	ld r1
	incr
	in p2
	ld r0
	incr
	in p2
	out p5
	incr
	in p2
	out p4
	incr
	
	pushs
	popb
	in p2
	pushd
	incr
	in p2
	pushd
	incr
	in p2
	pushd
	incr
	incr
	incr
	incr
	in p2
	pushd	; SP high
	decr
	in p2
	pushd	; SP low
	decr
	in p2
	pushd	; BP High
	decr
	in p2
	pushd	; BP low
	
	std 1
	sbp
	out p1
	std 2
	sbp
	out p0
	stl r7
	idc
	std 3
	sbp
	
	popb
	pops
	ei
iret
; ------------------

; ISR 3 ------------
Unknown:

iret
; ------------------

ConfigTimer:
	std $FA			; ACKnowledge Response
	ld r0
	std 0x01		; Abrir dispositivo no controller
	out p7
	call WaitACK
	std 0x02		; Escolher Dispositivo Timer -> Command: 0x01, Data: 0x02
	out p7
	call WaitACK
	call WriteTimerLimit
ret

WriteTimerLimit:
	std 0x02		; Ler dispositivo no controller
	out p7
	call WaitACK
	std 0x02		; Escolher Dispositivo Timer -> Command: 0x02, Data: 0x02
	out p7
	call WaitACK
	
	std $00
	out p7
	call WaitACK
	std $20
	out p7
	call WaitACK
	std $FF
	out p7
	call WaitACK
	std $FF
	out p7
	call WaitACK
ret

WaitACK:
	in p7
	bt r0
	jz ACK.Done
	jp WaitACK
ACK.Done:
	ret
	
ConfigKeyboard:
	std $FA			; ACKnowledge Response
	ld r0
	std 0x01
	out p7
	call WaitACK
	std 0x00
	out p7
	call WaitACK
ret

ReadKey:
	std key_buff::8
	out p0
	std key_buff::0
	out p1
	in p2
ret

ClearKey:
	cdr
	out p2
ret

ExitProcess:
	std .ProcessCall::8
	pushd
	std .ProcessCall::0
	pushd
	stl r0
	ret

CreateProcess:
	push r0
	push r1
	call set_proc_table
	std 1
	ld r3
	cdr
	ld r2
.LoopCreate:
	std 0x01
	ld r7
	idc
	std 22
	call sum_address
	
	in p2
	ld r7
	std 0x03
	bt r7
	jz .UpdateProc
	decr
	in p2
	ld r7
	incr
	std PID_END::0
	bt r7
	jz .ClearLastProc
	
	incr
	std $5B
	ld r7
	idc
	incr
	in p0
	ld r0
	in p1
	ld r1
	jp .LoopCreate
	
.ClearLastProc:
	std TABLE_SIZE
	bt r3
	jz .CreateError
	
.UpdateProc:
	pop r1
	pop r0
	cdr
	out p2
	decr
	stl r3
	out p2
	decr
	stl r0
	out p2
	decr
	stl r1
	out p2
	jp .CreateDone
	
.CreateError:
	ec
	pop r1
	pop r0
	ret
	
.CreateDone:
	dc
	ret

read_proc:
	in p2
	ld r1
	incr
	in p2
	ld r0
	incr
	in p2
	ld r4
	incr
	in p2
	ld r7
	call inc_proc_index
ret

calc_table:
	call mul_index_23
	
	push r0
	call set_proc_table
	pop r2
	
	call sum_address
	
	std 0x01
	idc
ret

set_proc_table:
	pushd
	std ProcTable::8
	ld r0
	out p0
	std ProcTable::0
	ld r1
	out p1
	popd
ret

set_proc_index:
	std ProcIndex::8
	out p0
	std ProcIndex::0
	out p1
ret

inc_proc_index:
	in p0
	pushd
	in p1
	pushd
	
	call set_proc_index
	std 0x80
	idc
	in p2
	incr
	out p2
	
	std 0x01
	idc
	popd
	out p1
	popd
	out p0
ret

multiply_by_23:
	pushd
	std 23
	ld r2
	popd
	mul r2
ret

mul_index_23:
	call set_proc_index
	in p2
	call multiply_by_23
ret

sum_address:
	pushd
	stl r0
	add r2
	ld r0
	popd
	add r1
	ld r1
	jc .incr0
	jp .ret_sum
.incr0:
	std 0x40
	idc
	incr
.ret_sum:
	stl r0
	out p0
	stl r1
	out p1
ret

; Input  -> R0:R1 : Absolute Address
;        -> R2:R3 : Jump Address
; Output -> R0:R1 : Jump + Relative Address
;CalcAddr:
;	pushd
;	std AddrCall::8
;	ld r2
;	out p0
;	std AddrCall::0
;	ld r3
;	out p1
	
;	std 2
;	add r3
;	ld r3
;	jc .incr2
;	jp .calcrel
;.incr2:
;	std 0x52
;	idc
;	incr

;.calcrel:
;	stl r0
;	sub r2
;	ld r0
;	stl r1
;	sub r3
;	ld r3
;	jc .skipinc
;	std 0x40
;	idc
;	decr
;.skipinc:
;	std $F0
;	or r0
;	ld r0
;	stl r3
;	ld r1
	
;	std 0x01
;	idc
;	stl r0
;	out p2
;	incr
;	stl r1
;	out p2
;	popd
;ret

