printf:
	pushb
	pushs
	popb

	clr
	st 1
	ld %r1
	st 4
	abp
	out %p1


.print:
	in %p2
	out %p3
	bt %r0
	jz .done

	in %p1
	add %r1
	out %p1
	jp .print

.done:
	cdr
	st 3
	shl 4
	ld %r1
	cdr
	st 5
	abp
	add %r1
	out %p3

	popb
ret
