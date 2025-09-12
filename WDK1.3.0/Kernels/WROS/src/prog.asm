ORG 0x12A

start:
	cdr
	st str::8
	out p0
	st str::4
	shl 4
	st str::0
	out p1
	call print
	dc
ret

str:
	db "my first program...",0

print:
	call setincone
	loop.prt:
		call checknull
		jz done.prt
		in p2
		out p3
		call incaddr
		jp loop.prt
done.prt:
	ret

setincone:
	cdr
	st 1
	ld r1
ret

checknull:
	cdr
	ld r3
	in p2
	bt r3
ret

incaddr:
	in p1
	add r1
	out p1
	jc 	.incrh
	ret
.incrh:
	in p0
	add r1
	out p0
ret
	