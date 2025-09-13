ORG 1707

start:
	cdr
	st str::8
	out p0
	st str::4
	shl 4
	st str::0
	out p1
	call print
	
	st 12
	;shl 4
	;st 7
	ld r0
	cdr
	st 7
	ld r1
	st 1
	shl 4
	st $E
	call printfloat
	
	pushb
	pops
	cdr
	st 2
	ssp
	
	dc
ret

str:
	db "Resultado : ",0

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

parsechar:
	pushd
	st 3
	shl 4
	pop r1
	or r1
	out p3
ret

printfloat:
	pushd
	push r1
	push r0
	call div8
	call parsechar
	
	cdr
	ld r3
	or r0
	bt r3
	jz done.float
	call printdot
.fploop:
	push r0
	cdr
	st 10
	pushd
	call mul8
	pop r0
	pop r0
	pop r0
	
	pushd
	call div8
	
	pushd
	cdr
	ld r3
	or r0
	bt r3
	popd
	ld r3
	jz .retchar
	
	popd
	pop r2
	cdr
	st 1
	ld r1
	popd
	sub r1
	pushd
	push r2
	pushd
	jc .prtnum
	jp .prtround
	
.prtnum:
	cdr
	or r3
	call parsechar
	jp .fploop
	
.prtround:
	cdr
	or r3
	add r1
	call parsechar
	jp done.float
.retchar:
	call parsechar
done.float:
	popd
	popd
	popd
ret

printdot:
	st 2
	shl 4
	st $E
	out p3
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

include "../../../libraries/sys8/asm/math.inc"
	