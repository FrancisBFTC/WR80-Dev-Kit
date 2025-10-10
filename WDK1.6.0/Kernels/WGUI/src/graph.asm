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
	std _titlew::8
	out p0
	std _titlew::0
	out p1
	call DrawWindow
	
.wait_msg1:
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
	std 81
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew::8
	out p0
	std _titlew::0
	out p1
	call DrawWindow
	
.wait_msg2:
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
	std 161
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew::8
	out p0
	std _titlew::0
	out p1
	call DrawWindow
	
.wait_msg3:
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
include "thread.asm"

END:
	