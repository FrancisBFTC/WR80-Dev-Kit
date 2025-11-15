include "examples/times2.asm"

db 00
rep 3
	rep 2
		rep 2
			db $AA
			db $BB
		endp
		db $01
	endp
	db $02
endp
db $FF
