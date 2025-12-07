Start:
	st Global::8
	out p0
	st Global::4
	shl 4
	st Global::0
	out p1
	call Hello
	
end:
	jp end
	
Hello:
	pushb
	pushs
	popb	
	cdr
	
	st 1
	ld r1
	
	st 11
	ssp			; SP = SP - DR
	
	ed
	dd
.print:
	pushd
	sbp			; DR = [BP - DR]
	out p2 		; Para debug externo
	out p3		; P3 = DR
	popd
	sub r1
	jz .print.done
	jp .print

.print.done:
	pushb
	pops
	popb
ret


Global:
	db 0
; Hello World = 48 65 6c 6c 6f 20 57 6f 72 6c 64
	