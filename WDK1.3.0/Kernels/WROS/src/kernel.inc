readkey:
	in p3
	bt r0
	jz readkey
ret

writechar:
	out p3
	out p2
	call setincone
	call incaddr
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

incregs:
	cdr
	st 1
	add r1
	ld r1
	jc 	.inchr
	ret
.inchr:
	cdr
	st 1
	add r0
	ld r0
ret

configcli:
	st 0x02
	shl 4
	st 0x04
	out p3
	st 0
	out p3
	
	call configbuff
ret

configbuff:
	cdr
	st clibuff::8
	out p0
	st clibuff::4
	shl 4
	st clibuff::0
	out p1
ret

getaddr:
	in p0
	ld r0
	in p1
	ld r1
ret

setaddr:
	cdr
	or r0
	out p0
	cdr
	or r1
	out p1
ret

saveregs:
	cdr
	or r0
	ld r2
	cdr
	or r1
	ld r3
ret

getregs:
	cdr
	or r2
	ld r0
	cdr
	or r3
	ld r1
ret

getcmdaddr:
	st cmd.vec::8
	ld r0
	st cmd.vec::4
	shl 4
	st cmd.vec::0
	ld r1
ret

getcliaddr:
	cdr
	st clibuff::8
	ld r0
	st clibuff::4
	shl 4
	st clibuff::0
	ld r1
ret

getbreak:
	pushd
	cdr
	st 0x0D
	ld r0
	popd
	bt r0
ret

setincone:
	cdr
	st 1
	ld r1
ret

setinctwo:
	cdr
	st 2
	ld r1
ret

checknull:
	cdr
	ld r3
	in p2
	bt r3
ret

strcmp:
	push r2
	push r3
	cmp.loop:
		call checknull
		jz cmp.end
		pushd
		
		push r0
		push r1
		call getaddr
		call incregs
		call saveregs
		pop r1
		pop r0
		
		call setaddr
		in p2
		pushd
		
		call getaddr
		call incregs
		push r0
		push r1
		call getregs
		call setaddr
		pop r1
		pop r0
		
		pushd
		pop r2
		bt r2
		jz cmp.loop
		
		pop r3
		pop r2
		ec
		ret
cmp.end:
	pop r3
	pop r2
	dc
	ret

clibuff:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
