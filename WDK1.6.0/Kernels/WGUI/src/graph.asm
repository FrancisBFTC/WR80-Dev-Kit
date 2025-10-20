jp Main

include "strlib.asm"
include "graphdat.asm"
include "graphlib.asm"
;include "font5x5.asm"
include "font8x8.asm"

win1_posxy:
	dw 0
win2_posxy:
	dw 0
win3_posxy:
	dw 0
	
Main:
	call SetStr
	call Print
	call DrawBackGround
	
	call StartTasks
	jp $FFF
	
Process1:
	ei
	pushs
	popb
	std MIN_ALLOC
	ssp
	
	std win1_posxy::8
	out p0
	pushd
	std win1_posxy::0
	out p1
	pushd
	call SetPosition
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	call CreateWindow
	std _1
	ld r0
	call VisualEffect
	
	pop r7
	pop r6
	call CloseWindow
	
	di
	pushb
	pops
	cdr
	dc
ret

Process2:
	ei
	pushs
	popb
	std MIN_ALLOC
	ssp
	
	std win2_posxy::8
	out p0
	pushd
	std win2_posxy::0
	out p1
	pushd
	call SetPosition
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	call CreateWindow
	std _2
	ld r0
	call VisualEffect
		
	pop r7
	pop r6
	call CloseWindow

	di
	pushb
	pops
	cdr
	dc
ret

Process3:
	ei
	pushs
	popb
	std MIN_ALLOC
	ssp
	
	std win3_posxy::8
	out p0
	pushd
	std win3_posxy::0
	out p1
	pushd
	call SetPosition
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	call CreateWindow
	std _3
	ld r0
	call VisualEffect
	
	pop r7
	pop r6
	call CloseWindow
	
	di
	pushb
	pops
	cdr
	dc
ret

CreateWindow:
	in p0
	pushd
	in p1
	pushd
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std DARK_GRAY2		; WHITE para botões
	ld r6
	call DrawWindow
	popd
	out p1
	popd
	out p0
ret

CloseWindow:
	call ResetPosition
	std _R1
	ld r7
	idc
	incr
	
	std _R2
	ld r7
	idc
	incr
	
	std _R4
	ld r7
	idc
	decr
	
	std _R5
	ld r7
	idc
	decr
	
	std BACKCOLOR
	ld r6
	call DrawSolidSquare
ret

SetPosition:
	in p0
	pushd
	in p1
	pushd
	
	std pos_x::8
	out p0
	std pos_x::0
	out p1
	in p2
	ld r4
	
	std pos_y::8
	out p0
	std pos_y::0
	out p1
	in p2
	ld r5
	
	popd
	out p1
	popd
	out p0
	std P0_P1
	ld r7
	idc
	stl r4
	out p2
	incr
	stl r5
	out p2
ret

ResetPosition:
	push r4
	push r5
	
	stl r6
	out p0
	stl r7
	out p1
	
	std P0_P1
	ld r7
	idc
	in p2
	ld r4
	incr
	in p2
	ld r5
	
	std pos_x::8
	out p0
	std pos_x::0
	out p1
	stl r4
	out p2
	
	std pos_y::8
	out p0
	std pos_y::0
	out p1
	stl r5
	out p2
	
	pop r5
	pop r4
ret

VisualEffect:
	push r1
	push r2
	push r4
	push r5
	
	call CounterEffect
	
.BeginEffect:
	std WHITE
	ld r6
.LoopEffect:
	push r0
	
	push r4
	std _R6
	ld r7
	idc
	incr
	std _strwin::8
	out p0
	std _strwin::0
	out p1
	call PrintString
	pop r4
	
	pop r0
	call ReadKey
	bt r0
	jz .LoopEffect.ret
	jp .LoopEffect
	
.LoopEffect.ret:
	call ClearKey
	pop r5
	pop r4
	pop r2
	pop r1
ret

CounterEffect:
	push r0
	push r1
	push r2
	push r3
	push r6
	push r7
	
	std TOP_SIZE
	add r5
	ld r5
	std 1
	add r4
	ld r4
	cdr
	ld r1
	ld r2
	ld r6
	std _COUNT
	ld r3
.BeginCount:
	call ShowNum
	
.LoopCounter:
	std R1_R2
	ld r7
	idc
	incr
	std $FF
	ld r0
	stl r2
	and r1
	bt r0
	jz .IncCountHigh
	jp .LoopCounter
.IncCountHigh:
	std _R6
	ld r7
	idc
	incr
	std $1
	bt r6
	jc .LoopCounter
	
	cdr
	ld r6
	
	std _R3
	ld r7
	idc
	decr
	std _0
	bt r3
	jz .CounterDone
	jp .BeginCount
.CounterDone:
	std FONT_SIZE
	ld r1
	ld r2
	std DARK_GRAY
	ld r6
	call DrawSolidSquare
	
	pop r7
	pop r6
	pop r3
	pop r2
	pop r1
	pop r0
ret
	
ShowNum:
	push r0
	push r1
	push r2
	push r3
	push r6
	push r7
	
	std FONT_SIZE
	ld r1
	ld r2
	std DARK_GRAY
	ld r6
	call DrawSolidSquare
	
	std BLUE
	ld r6
	stl r3
	call PrintChar
	
	pop r7
	pop r6
	pop r3
	pop r2
	pop r1
	pop r0
ret

include "thread.asm"
	