Start:
	;call Hello1
	call Hello2
	
end:
	jp end
	
Hello1:
	pushb
	pushs
	popb	
	cdr
	
	st 1
	ld r1
	
	st 11
	ssp			; SP = SP - DR
	
	call CreateString1
	
.print1:
	pushd
	sbp			; DR = [BP - DR]
	out p2 		; Para debug externo
	out p3		; P3 = DR
	popd
	sub r1
	jz .print1.done
	jp .print1

.print1.done:
	pushb
	pops
	popb
ret

Hello2:
	pushb
	pushs
	popb	
	cdr
	
	st 1
	ld r1
	
	st 11
	ssp			; SP = SP - DR
	
	call CreateString2
	
.print2:
	pushd
	sbp			; DR = [BP - DR]
	out p2 		; Para debug externo
	out p3		; P3 = DR
	popd
	sub r1
	jz .print2.done
	jp .print2

.print2.done:
	pushb
	pops
	popb
ret

CreateString1:
	pushd
	pushd
	st 4
	shl 4
	st 8
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st 5
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st $c
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st $c
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st $f
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 2
	shl 4
	st 0
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 5
	shl 4
	st 7
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st $f
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 7
	shl 4
	st 2
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st $c
	ld r2
	popd
	sbw
	sub r1
	
	pushd
	st 6
	shl 4
	st 4
	ld r2
	popd
	sbw
	popd
ret

CreateString2:
	pushd
	pushd
	cdr
	st String::8
	out p0
	st String::4
	shl 4
	st String::0
	out p1
	.loop.create:
		in p2
		ld r2
		popd
		sbw
		sub r1
		bt r0
		jz .create.done
		
		pushd
		in p1
		add r1
		out p1
		jp .loop.create
		
.create.done:
	popd
ret

String:
	db "Hello World"
	
; Hello World = 48 65 6c 6c 6f 20 57 6f 72 6c 64
