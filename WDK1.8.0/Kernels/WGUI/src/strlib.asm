Print:
	call IncConf
	.loop.prt:
		call CheckNull
		jz .done.prt
		in p2
		out p3
		incr
		jp .loop.prt
.done.prt:
	ret
	
IncConf:
	std P0_P1
	ld r7
	idc
ret

CheckNull:
	cdr
	ld r3
	in p2
	bt r3
ret

SetStr:
	std _string::8
	out p0
	std _string::0
	out p1
ret
