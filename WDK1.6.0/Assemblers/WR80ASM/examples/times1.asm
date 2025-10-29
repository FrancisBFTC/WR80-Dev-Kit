macro .times m, v
	rep #m
		db #v
	endp
endm

macro .times m, n, v
	rep #m
		rep #n
			db #v
		endp
	endp
endm

.times 16, 0
.times 4, 2, $01
.times 4, 2, $02
.times 8, 3
.times 8, 4
