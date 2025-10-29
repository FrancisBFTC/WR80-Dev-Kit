macro .times ...
	rep #.
		rep #-
			db #.
		endp
		rep #.
		endp
	endp
endm

.times 4, 1, 2, 3
db $FF
.times 3, $AA, $BB
