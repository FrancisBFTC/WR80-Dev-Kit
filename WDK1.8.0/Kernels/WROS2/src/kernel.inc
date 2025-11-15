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

getchar:
	in p2
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
	add r1
	ld r1
	jc 	.inchr
	ret
.inchr:
	std 1
	add r0
	ld r0
ret

decregs:
	stl r3
	sub r1
	ld r3
	jc 	done.dec
	stl r2
	sub r1
	ld r2
	dc
done.dec:
	ret

getoffset:
	call incregs 	; incrementa R0:R1
	call setaddr 	; configura P0:P1
	in p2			; pega byte
ret

configcli:
	std 0x20
	pushd
	st 0x04
	out p3
	std $3E
	out p3
	popd
	out p3
	
	call configbuff
ret

configbuff:
	cdr
	st clibuff::8
	out p0
	std clibuff::0
	out p1
ret

configtable:
	std 0x11
	ld r0
	out p0
	cdr
	ld r1
	out p1
ret

getaddr:
	in p0
	ld r0
	in p1
	ld r1
ret

setaddr:
	stl r0
	out p0
	stl r1
	out p1
ret

saveregs:
	stl r0
	ld r2
	stl r1
	ld r3
ret

getregs:
	stl r2
	ld r0
	stl r3
	ld r1
ret

getcmdaddr:
	cdr
	st cmd.vec::8
	ld r0
	std cmd.vec::0
	ld r1
ret

getcommands:
	cdr
	st commands::8
	ld r0
	std commands::0
	ld r1
ret

getcliaddr:
	cdr
	st clibuff::8
	ld r0
	std clibuff::0
	ld r1
ret

setcmdevt:
	cdr
	st cmd.evt::8
	ld r0
	out p0
	std cmd.evt::0
	ld r1
	out p1
ret

getcmdsize:
	call getcmdaddr
	call saveregs
	call getcommands
	call sub16
	call divideby2
ret

divideby2:
	stl r1
	shr 1
	ld r1
	stl r0
	shl 7
	or r1
ret

getbreak:
	pushd
	std 0x0D
	ld r0
	popd
	bt r0
ret

getspace:
	pushd
	std 0x20
	ld r0
	popd
	bt r0
ret

getbackspace:

ret

breakline:
	std 0x0D
	out p3
	std $0A
	out p3
ret

setincone:
	std 1
	ld r1
ret

setinctwo:
	std 2
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
		std 1
		call incregs
		call saveregs
		pop r1
		pop r0
		
		call setaddr
		in p2
		pushd
		
		call getaddr
		std 1
		call incregs
		push r0
		push r1
		call getregs
		call setaddr
		pop r1
		pop r0
		
		popd
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
	
print:
	call setincone
	loop.prt:
		call checknull
		jz done.prt
		call getchar
		out p3
		call incaddr
		jp loop.prt
done.prt:
	ret

printhex4:
	push r2
	push r3
	pushd
	std 0x30
	ld r0
	std 0x40
	ld r1
	std 10
	ld r2
	std 1
	ld r3
	popd
	bt r2
	jc isalpha
	add r0
	jp done.hex
isalpha:
	sub r2
	add r3
	add r1
done.hex:
	out p3
	pop r3
	pop r2
	ret
	
printhex8:
	pushd
	shr 4
	call printhex4
	popd
	shl 4
	shr 4
	call printhex4
ret

printsize:
	push r0
	push r1
	push r2
	std 12
	call incregs
	call setaddr
	call getchar
	pushd
	call setincone
	call incaddr
	call getchar
	pushd
	call setsizestr
	call print
	popd
	call printhex4
	popd
	call printhex8
	pop r2
	pop r1
	pop r0
ret

printerr:
	cdr
	st errorstr::8
	out p0
	std errorstr::0
	out p1
	call print
ret

setfile:
	cdr
	ld r0
	st 2
	ld r1
ret

copyname:
	call setfile
	call saveregs
	call configbuff
	copy:
		push r3
		call checknull
		pop r3
		jz done.cpy
		pushd
		call getaddr
		std 1
		call incregs
		push r0
		push r1
		call getregs
		call setaddr
		std 1
		call incregs
		call saveregs
		pop r1
		pop r0
		popd
		out p2
		call setaddr
		jp copy
done.cpy:
	call getregs
	call setaddr
	cdr
	out p2
	ret

configwrofs:
	pushd
	call setfile
	std 10
	call incregs
	call setaddr
	popd
	out p2
	std $F2
	out p1
	cdr
	out p0
	st 0xE
	out p2
ret

sub16:
	stl r0
	sub r2
	ld r0
	
	stl r1
	sub r3
	ld r1
	
	jc sub.end

	std 1
	ld r2
	stl r0
	sub r2
	ld r0
	
sub.end:
	ret
	
sub8:
	stl r2
	sub r1
	ld r2
ret

configevt:
	pushb
	pushs
	popb
	
	std 2
	call incregs
	call saveregs	; R2:R3 = PC + 2
	
	std 4
	abp
	ld r0
	
	std 5
	abp
	ld r1
	
	call sub16		; Label - (PC + 2)

	std $F0
	or r0
	out p2
	
	push r1
	call setincone
	call incaddr
	popd
	out p2
	
	popb
ret

formatcli:
	cdr
	out p2
	ld r2
	call configbuff
	call setincone
	init:
		call checknull
		jz done.format
		call getspace
		jz writezero
		jp backinit
	writezero:
		cdr
		out p2
		st 1
		add r2
		ld r2
	backinit:
		call incaddr
		jp init
done.format:
	ret
	
setsizestr:
	cdr
	st sizestr::8
	out p0
	std sizestr::0
	out p1
ret

getarg:
	pushd
	std 1
	ld r1
	popd
	sub r1
	jz done.getarg
	pushd
geta.loop:
	call getaddr
	std 1
	call getoffset
	call checknull
	jz geta.incr
	jp geta.loop
geta.incr:
	std 1
	call getoffset
	popd
	jp getarg
done.getarg:
	ret

sethexarg:
	cdr
	st hexarg::8
	ld r0
	std hexarg::0
	ld r1
ret

setinfo:
	cdr
	st info::8
	out p0
	std info::0
	out p1
ret
		
sizestr:
	db 0x09,"size: 0x",0
clibuff:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	