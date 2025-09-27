somar:
	pushb
	pushs
	popb

	st 4
	abp
	pushd
	cdr

	st 5
	abp
	pushd
	cdr

	st 6
	abp
	pop r0
	add r0
	pop r0
	add r0

	popb
ret
