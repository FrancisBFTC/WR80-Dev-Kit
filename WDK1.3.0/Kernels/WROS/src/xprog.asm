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
	
	st 7
	ld r0
	cdr
	st 4
	ld r1
	;st $F
	;shl 4
	;st $F
	;call printfloat
	push r1
	push r0
	call div8
	pop r1
	pop r1
	call EncodeFloat8
	ed
	
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

parsechar:
	pushd
	st 3
	shl 4
	pop r1
	or r1
	out p3
ret

define BIAS	7



;-----------------------------------------
; Função: EncodeFloat8
; Entrada:
;   DR = parte inteira do quociente (0..255)
;   R0 = resto (0..255)
;	R1 = divisor ou denominador (0..255)
; Saída:
;   DR = valor em formato float8 (S EEE MMMM)
;-----------------------------------------

EncodeFloat8:
	pushd
	cdr
	ld r3
	or r0
	bt r3
	jz Mantissa.gen
	popd
	
	pushd
	call GetNextDecimal
	
	pushd 				; Dec = 7
	call GetNextDecimal
	ld r3				; Dec = 5
	popd
	ld r0				; R0 = 7
	
	push r3
	call MulRemindBy10
	pop r3
	add r3				; 7 x 10 + 5
	
	
Mantissa.gen:
	pushd				; 75
	cdr
	ld r4
	st 2	; 2 + 1 = 3 bits for mantissa
	ld r3
	popd
	Mantissa.loop:
		push r3
		call MulDecPlaceBy2
		call DivProdBy100		; DivProdBy10
		
		; DR = Quociente -> Bit mantissa
		; R0 = Resto ou Sobra (nova fracao)
		call StoreMantissa
		cdr
		or r0
		
		pop r3
		call DecMantCount
		jc Mantissa.loop
		
Expoent.gen:
	st 8
	shl 4
	ld r5
	cdr
	ld r3
	st 1
	ld r1
	cdr
	or r4
	shl 4
	ld r4
	popd
	bt r3
	pushd
	jz Dot.shr
	bt r1
	popd
	jz end.expoent
	
Dot.shl:
	pushd
	and r1
	jz only.shl
	cdr
	or r4
	shr 1
	or r5
	jp shift.int
only.shl:
	cdr
	or r4
	shr 1
shift.int:
	ld r4
	cdr
	or r3
	add r1
	ld r3
	popd
	shr 1
	bt r1
	jz end.expoent
	jp Dot.shl
	
Dot.shr:
	cdr
	or r3
	sub r1
	ld r3
	cdr
	or r4
	shl 1
	ld r4
	popd
	jc end.expoent
	pushd
	jp Dot.shr
	
end.expoent:
	; R3 = Quantidade de deslocamentos (expoente positivo/negativo)
	; R4 = Mantissa na parte alta
	; DR = Parte inteira (1 normalizado)
	
	cdr
	or r4
	shr 4
	ld r4
	cdr
	st BIAS
	add r3
	ld r3
	cdr
	st $F
	and r3
	shl 3
	or r4
ret
	
; ============================
; DecodeFloat8 (WR80)
; Entrada: DR = byte FLOAT8 [S|EEE|MMMM], bias=7, mantissa 3 bits
; Saída: imprime "INT.DD" com 2 casas decimais via OUT P3
; Usa: R0..R7 temporários; salva R4..R7 no início
; ============================

DecodeFloat8:

	; Inicia registradores
	ld r1
	cdr
	st BIAS
	ld r2
	cdr
	st 8
	shl 4
	ld r3
	cdr
	st $F
	shl 4
	ld r0
	
	; Ler expoente
	and r0
	shr 4
	sub r2				; Subtrai expoente - bias
	jz mant.noshift		; Se for zero...
	jc mant.shl			; Se for positivo...
	
mant.shr:
	; Se expoente negativo, desloca mantissa pra direita
	ld r0
	not r0
	cdr
	st 1
	ld r2
	add r0
	ld r0
	cdr
	st 7
	and r1
	shl 4
	or r3	; bit implícito de normalização
	ld r1
	shr.loop:
		cdr
		or r1
		shr 1
		ld r1
		cdr
		or r0
		sub r2
		jz done.shift
		jp shr.loop
		
mant.shl:
	; Se expoente positivo, desloca mantissa pra esquerda
	cdr
	st 7
	and r1
mant.noshift:
	; Se expoente for zero, não desloca mantissa
	cdr
	st 7
	and r1
ret

GetNextDecimal:
	call MulRemindBy10
	call DivRemindByDivisor
ret

MulRemindBy10:
	push r1
	push r0
	cdr
	st 10
	pushd
	call mul8
	pop r0
	pop r0
	pop r1
ret

DivRemindByDivisor:
	push r1
	pushd
	call div8
	pop r2
	pop r1
ret

MulDecPlaceBy2:
	pushd
	cdr
	st 2
	pushd
	call mul8
	pop r0
	pop r0
ret

DivProdBy10:
	pushd
	cdr
	st 10
	ld r0
	popd
	push r0
	pushd
	call div8
	pop r2
	pop r1
ret

DivProdBy100:
	pushd
	st 6
	shl 4
	st 4
	ld r0
	popd
	push r0
	pushd
	call div8
	pop r2
	pop r1
ret

DecMantCount:
	pushd
	cdr
	st 1
	ld r1
	cdr
	or r3
	sub r1
	ld r3
	popd
ret

StoreMantissa:
	pushd
	cdr
	or r4
	shl 1
	ld r4
	popd
	or r4
	ld r4
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
	