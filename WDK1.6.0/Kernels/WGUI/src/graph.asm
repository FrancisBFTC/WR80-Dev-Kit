jp Main

include "thread.asm"

Main:
	call SetStr
	call Print
	call DrawBackGround
	
	call StartTasks
	jp END
	
Process1:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std win1_posxy::8
	out p0
	std win1_poswy::0
	out p1
	call SetPosition
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	call CreateWindow
	std 0x31
	ld r0
	call VisualEffectTitle
	
	di
	call CloseWindow
	
	pushb
	pops
	std 0
	ld r0
	call ExitProcess
ret

Process2:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std win2_posxy::8
	out p0
	std win2_poswy::0
	out p1
	call SetPosition
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	call CreateWindow
	std 0x32
	ld r0
	call VisualEffectTitle
	
	di
	call CloseWindow
	
	pushb
	pops
	std 0
	ld r0
	call ExitProcess
ret

Process3:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std win3_posxy::8
	out p0
	std win3_poswy::0
	out p1
	call SetPosition
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	call CreateWindow
	std 0x33
	ld r0
	call VisualEffectTitle
	
	di
	call CloseWindow
	
	pushb
	pops
	std 0
	ld r0
	call ExitProcess
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
	std WHITE
	ld r6
	call DrawWindow
	popd
	out p1
	popd
	out p0
ret

CloseWindow:
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
	
	std BLUE
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

VisualEffectTitle:
	push r1
	push r2
	std WHITE
	ld r6
.wait_msg:
	in p0
	pushd
	in p1
	pushd
	push r0
	
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	
	pop r0
	call ReadKey
	bt r0
	jz .wait_msg.ret
	
	popd
	out p1
	popd
	out p0
	jp .wait_msg
.wait_msg.ret:
	call ClearKey
	popd
	out p1
	popd
	out p0
	pop r2
	pop r1
ret

win1_posxy:
	dw 0
win2_posxy:
	dw 0
win3_posxy:
	dw 0

include "graphdat.asm"
include "graphlib.asm"
include "strlib.asm"
;include "font5x5.asm"
include "font8x8.asm"

END:
	