Main:
	st 3
	pushd
	st 2
	pushd
	st 1
	pushd
	call MinhaFuncao
	pushb
	pops
	
	pushd
	st VarGlobal::8
	out p0
	st VarGlobal::4
	shl 4
	st VarGlobal::0
	out p1
	popd
	out p2
	
end:
	jp end
	
; BP - 1 -> VarLocal => Variavel Local
; BP + 0 -> Endereço de BP original 
; BP + 2 -> Endereço da função MinhaFunção
; BP + 4 -> Argumento '1'
; BP + 5 -> Argumento '2'
; BP + 6 -> Argumento '3'
MinhaFuncao:
	pushb
	pushs
	popb
	
	; R0 = [BP + 4]
	cdr
	st 4
	abp		; DR = [BP + DR]
	ld r0	; R0 = DR = [BP + DR] = 1
	
	; R1 = [BP + 5]
	cdr
	st 5
	abp		; DR = [BP + DR]
	ld r1 	; R1 = DR = [BP + DR] = 2
	
	; R2 = [BP + 6]
	cdr
	st 6
	abp		; DR = [BP + DR]
	ld r2	; R2 = DR = [BP + DR] = 3
	
	cdr
	or r0
	add r1
	add r2
	
	pushd
	st 1
	sbp
	
	pushb
	pops
	popb
ret

VarGlobal:
	db 0
