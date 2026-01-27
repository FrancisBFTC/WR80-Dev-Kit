include "../../libraries/wr80asm/wr80x.asm"
include "../../libraries/sys8/asm/math.inc"

; ---------------------------------------------------------
; Função: PrintFloat
; Entradas:
; 	R3 = parte inteira somada ou zero
; 	R0 = Dividendo
; 	R1 = Divisor
; 	R5 = Saída padrão: 0 -> monitor; 1 -> memória/arquivo
; 	DR = Casas decimais limites
; ---------------------------------------------------------

PrintFloat:
	pushd
	push r1
	push r0
	call div8
	
	call parseinteger
	
	cdr
	ld r3
	st 1
	ld r4
	stl r0
	bt r3
	jz done.float
	call parsedot
.fploop:
	push r0
	std 10
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
	stl r0
	bt r3
	popd
	ld r3
	jz .retchar
	
	popd
	pop r2
	std 1
	ld r1
	popd
	sub r1
	jc .prtnum
	jp .prtround
	
.prtnum:
	pushd
	call decreasediv
	push r2
	pushd
	stl r3
	call parsenum
	jp .fploop
	
.prtround:
	stl r3
	add r1
	call parsenum
	ret
.retchar:
	call parsenum
done.float:
	popd
	popd
	popd
ret

; ---------------------------------------------------------
; Rotinas auxiliares para PrintFloat

decreasediv:
	cdr
	bt r4
	jz .nodecdiv
	cdr
	ld r4
	stl r2
	shr 2
	ld r2
	stl r0
	shr 2
	ld r0
.nodecdiv:
	ret

; Processar parte inteira
parseinteger:
	add r3		; Soma quociente com R3
	ld r3		; Coloca em R3
	std 0x64
	ld r2		; R2 = 100
	std 1
	ld r1		; R1 = estado de zeros
	push r0		; Salva resto da primeira divisão
	
integer.loop:
	push r2
	push r3
	call div8
	
	pushd
	cdr
	ld r3
	bt r1
	popd
	jz printdigit
	bt r3
	jz divBy10
	
printdigit:
	call parsenum
	pushd
	cdr
	ld r1
	popd
	
divBy10:
	pop r3
	pop r2
	stl r0
	ld r3
	pushd
	std 1
	bt r2
	popd
	jz checklastzero
	
	std 10
	pushd
	push r2
	call div8
	ld r2
	popd
	popd
	jp integer.loop

checklastzero:
	pushd
	cdr
	bt r1
	popd
	jz noprtzero
	
	call parsenum
	
noprtzero:
	pop r0
ret
; ---------------------------------------------------------

; ---------------------------------------------------------
parsenum:
	pushd
	std 0x30
	pop r1
	or r1
	call writechar
ret

parsedot:
	std $2E
	call writechar
ret

writechar:
	pushd
	cdr
	bt r5
	jz prttty
	popd
	out p2
	push r1
	stl r5
	ld r1
	std 0x01
	idc
	incr
	pop r1
	cdr
	out p2
	jp ret.wchar
prttty:
	popd
	out p3
ret.wchar:
	ret
; ---------------------------------------------------------

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
	pushd				; Quociente da divisão
	pushd				; Decimal de 2 casas decimais, Ex.: 75
	std 1
	ld r2
	cdr
	ld r4				; R4 = 0
	st 7				; 7 + 1 = 3 bits pra mantissa (o loop processa +1)
	ld r3				; R3 = 2
	popd				; Restaura as 2 casas
	
	Mantissa.loop:
		push r3					; Salva contador
		call ExtractBit			; Multiplica resto por 2 e define Bit
								; Se for maior ou igual que divisor
								; DR = Bit mantissa, R0 = nova fracao
		call StoreMantissa		; Armazena bit em R4
		
		pop r3					; Restaura contador
		call DecMantCount		; Subtrai contador - 1
		jc Mantissa.loop		; Se for positivo, continue loop
		
; R4 = Mantissa de 3 bits na parte alta

Expoent.gen:
	std 0x80
	ld r5				; R5 = 10000000b
	cdr
	ld r3				; R3 = 0
	st 1
	ld r1				; R1 = 1
	popd
	bt r3				; Compara quociente com 0
	jz Dot.shr			; Se for igual, deslocar ponto pra direita (Bits pra esquerda)
	bt r1				; Compara quociente com 1
	jz end.expoent		; Se for igual, já está normalizado, não precisa deslocar
	
; Se quociente != 1 e != 0, o ponto é deslocado pra esquerda
; Ou seja, bits pra direita (expoente positivo)
Dot.shl:
	pushd
	and r1				; Verifica se bit 0 = 1
	jz only.shl			; Se não, só desloca bits 
						; Se sim,...
	stl r4				; Pega bits da mantissa, DR = R4
	shr 1				; Desloca 1 bit pra direita
	or r5				; Define bit alto, DR = (R4 >> 1) | R5
	jp shift.int
only.shl:
	stl r4				; Pega bits da mantissa
	shr 1				; Desloca 1 bit pra direita
shift.int:
	ld r4				; Rearmazena em R4, R4 = DR
	stl r3				; Ler R3
	add r1				; Adiciona +1
	ld r3				; Salva em R3
	popd
	shr 1				; Desloca DR 1 bit
	bt r1				; Compara com 1
	jz end.expoent		; Se for igual, está normalizado
	jp Dot.shl			; Se não, volta pro loop
	
; Desloca ponto para a direita, ou seja,
; os bits são deslocados para a esquerda (expoente negativo)
Dot.shr:
	stl r3
	sub r1
	ld r3
	stl r4
	shl 1
	ld r4
	jc end.expoent
	jp Dot.shr
	
end.expoent:
	; R3 = Quantidade de deslocamentos (expoente positivo/negativo, 0 para normal)
	; R4 = Mantissa na parte alta
	
	stl r4		; Ler mantissa
	shr 5		; Coloca na parte baixa
	ld r4		; Carrega em R4
	std BIAS	; Define o Viés
	add r3		; Adiciona + o expoente
	ld r3		; Carrega em R3
	std $F		; Define valor de isolamento (parte baixa)
	and r3		; Isola a parte baixa do expoente
	shl 3		; Coloca na posição correta do float
	or r4		; Mescla com a mantissa
ret
	
;-----------------------------------------
; Função: DecodeFloat8
; Entrada: 
;	DR = byte FLOAT8 [S|EEEE|MMM]
; Saída: 
;	R0 = mantissa / 2 ou mantissa / 8
;	R1 = Divisor -> 128 ou 32
;	R3 = Parte inteira
;	
;-----------------------------------------

DecodeFloat8:

	; Inicia registradores
	ld r1				; R1 = Float8
	std BIAS
	ld r2				; R2 = Viés
	std 0x80
	ld r3				; R3 = 10000000b
	std 0x78			; Bits de isolamento
	ld r0				; R0 = 0x78 = 01111000b
	
	; Ler expoente armazenado
	stl r1				; DR = Float8
	and r0				; Isola os bits do expoente
	shr 3				; Desloca pra parte baixa
		
	sub r2				; Subtrai expoente - bias = expoente real
	jz mant.noshift		; Se for zero, não desloca
	jc mant.shl			; Se for positivo, desloca pra esquerda
						; Se for negativo, desloca pra direita
						
mant.shr:
	; Se expoente negativo, desloca mantissa pra direita
	ld r0				; Carrega em R0 expoente real
	not r0				; Inverte bits do expoente real
	ld r0				; Carrega de volta em R0
	std 1				; Define valor 1 pra decremento
	ld r2				; R2 = 1
	add r0				; Realiza complemento de 2 (conversão pra positivo)
	ld r0				; R0 = expoente positivo pra decremento
	
	std 7				; Define bits de isolamento da mantissa
	and r1				; Isola a mantissa
	shl 5				; Coloca mantissa na parte alta
	ld r1				; R1 = mantissa na parte alta
	
	shr.loop:
		stl r1			; Ler mantissa
		shr 1			; Desloca 1 bit pra direita
		or r3			; Define bit implícito da normalização
		ld r1			; Atualiza mantissa
		cdr				; DR = 0
		ld r3			; R3 = DR = 0 (limpa bit implícito)
		stl r0			; Ler expoente
		sub r2			; Subtraia com 1
		ld r0			; Atualiza expoente
		jz done.shift	; Se for zero, termine
		jp shr.loop		; Se não, continue
		
; Se expoente positivo, desloca mantissa pra esquerda
mant.shl:
	ld r0				; Carrega em R0 expoente real
	stl r3				; ler valor de R3 (10000000)
	shr 7				; Coloca o 1 na parte baixa (bit implícito = 1)
	ld r3 				; Carrega novamente em R3
	ld r2				; R2 = 1 (para decrementos)
	
	std 7				; Define bits de isolamento da mantissa
	and r1				; Isola a mantissa
	shl 5				; Coloca mantissa na parte alta
	ld r1				; R1 = mantissa na parte alta
	
	shl.loop:
		stl r3			; Ler bit implícito (normalizado)
		shl 1			; Desloca 1 bit pra esquerda
		ld r3			; Atualiza parte inteira
		stl r1			; Ler mantissa
		shl 1			; Desloca 1 bit pra esquerda
		ld r1			; Atualiza mantissa
		jc .def_bit		; Se deu overflow, defina bit na parte inteira
		jp .skipdbit	; Se não, o bit<0> = 0
	.def_bit:
		stl r3			; Ler parte inteira
		or r2			; Define bit<0> = 1
		ld r3			; Atualiza parte inteira
	.skipdbit:
		stl r0			; Ler expoente
		sub r2			; Subtraia com 1
		ld r0			; Atualiza expoente
		jz done.shift	; Se for zero, termine
		jp shl.loop		; Se não, continue
		
	
mant.noshift:
	; Se expoente for zero, não desloca mantissa
	stl r3		; ler valor de R3 (10000000)
	shr 7		; Coloca o 1 na parte baixa (bit implícito = 1)
	ld r3		; Carrega novamente em R3
	std 7		; Define bits de isolamento
	and r1		; Isola os 7 bits baixos da mantissa
	shl 5		; Coloca mantissa na parte alta
	ld r1		; Atualiza mantissa em R1

done.shift:
	std 7
	and r1
	jz .shift_3	; Se os 3 bits baixos for zero, divide por 8
				; Se não, a fração é baixa, então divide por 2
	stl r1
	shr 1
	ld r0		; R0 = valor da mantissa / 2
	std 0x80
	ld r1		; R1 = 256 / 2 = 128
	jp .skipshr3
.shift_3:
	or r1
	shr 3
	ld r0		; R0 = valor da mantissa / 8
	st 0x20
	ld r1		; R1 = 256 / 8 = 32
.skipshr3:
	cdr
	ret

PrintFloat8:
	call DecodeFloat8
	ld r5	; R5 = saída -> monitor
	st 8	; 8 casas decimais limites
	call PrintFloat
ret

ParseFloat8:
	call DecodeFloat8
	st 1
	ld r5	; R5 = saída -> arquivo/memória
	st 8	; 8 casas decimais limites
	call PrintFloat
ret

; --------------------------------------------
; Rotinas para o EncodeFloat8

ExtractBit:
	stl r0
	shl 1
	ld r0
	bt r1
	jc def_1
	cdr
	jp skipdef_1
def_1:
	stl r2
	pushd
	stl r0
	sub r1
	ld r0
	popd
skipdef_1:
	ret

DecMantCount:
	pushd
	stl r3
	sub r2
	ld r3
	popd
ret

StoreMantissa:
	pushd
	stl r4
	shl 1
	ld r4
	popd
	or r4
	ld r4
ret
; --------------------------------------------