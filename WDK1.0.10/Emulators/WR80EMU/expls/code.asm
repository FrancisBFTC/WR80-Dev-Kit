	st 0x0D
	out p1
	ed
readkey:
	in p3
	bt r0
	jz readkey
	out p3
	out p2
	ed
	jp readkey
