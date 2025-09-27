Main:
	call SetStr
	call Print
	call DrawBackGround
	
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
	
	jp END

include "graphdat.asm"
include "graphlib.asm"
include "strlib.asm"
include "font5x5.asm"
;include "font8x8.asm"

END:
	