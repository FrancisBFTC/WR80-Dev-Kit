readkey:
	in p3
	bt r0
	jz readkey
ret

writechar:
	out p3
	out p2
	call incraddr
ret

incraddr:
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

configcli:
	st 1
	ld r1
	st 0x02
	shl 4
	st 0x04
	out p3
	st 0
	out p3
	
	cdr
	st clibuff::8
	out p0
	st clibuff::4
	shl 4
	st clibuff::0
	out p1
ret
clibuff:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0
	