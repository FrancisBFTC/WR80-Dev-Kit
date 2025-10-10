jp Main

include "thread.asm"

Main:
	call SetStr
	call Print
	call DrawBackGround
	
	jp StartTasks
	jp END

Process1:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std POSX
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	call DrawWindow
	
	std GREEN
	ld r6
.wait_msg1:
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg1
.ret_proc1:
	pushb
	pops
	di
ret

Process2:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std 132
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	call DrawWindow
	
	std BLUE
	ld r6
.wait_msg2:
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg2
.ret_proc2:
	pushb
	pops
	di
ret

Process3:
	ei
	pushs
	popb
	std 0x20
	ssp
	
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std POSX
	ld r4
	std 52
	ld r5
	std WHITE
	ld r6
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	call DrawWindow
	
	std WHITE
	ld r6
	
.wait_msg3:
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg3
.ret_proc3:
	pushb
	pops
	di
ret

include "graphdat.asm"
include "graphlib.asm"
include "strlib.asm"
;include "font5x5.asm"
include "font8x8.asm"

END:
	