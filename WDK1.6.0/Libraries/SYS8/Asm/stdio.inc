define PARAM1 4
define PARAM2 5
define PARAM3 6
	
; Printf - Print the String in the TTY output
; In:
;	BP + 4 = High part string address
;	BP + 5 = Low part string address
; 	BP + 6 = number from 0 to 9
printf:
	pushb
	pushs
	popb

	clr
	st 1
	ld %r1
	st PARAM1
	abp
	out %p0
	cdr
	st PARAM2
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
	st PARAM3
	abp
	add %r1
	out %p3

	popb
ret

; Puts - Print the String with line break
; In:
;	BP + 4 = High part string address
;	BP + 5 = Low part string address	
puts:
	pushb
	pushs
	popb
	
	clr
	st 1
	ld %r1
	st PARAM1
	abp
	out %p0
	cdr
	st PARAM2
	abp
	out %p1

.puts_loop:
	in %p2
	out %p3
	bt %r0
	jz .puts_done

	in %p1
	add %r1
	out %p1
	jp .puts_loop

.puts_done:
	cdr
	st $0D
	out %p3
	
	popb
ret

; Getchar - get the first char from keyboard waiting Enter
; In: None
; Out:
;	DR = ASCII Character
getchar:
	pushb
	pushs
	popb
	
	; Configura endereco da posicao do buffer em P0:P1
	cdr
	st stdpos::8
	out p0
	st stdpos::4
	shl 4
	st stdpos::0
	out p1
	
	; Ler o numero da posicao, salva em R2 e na pilha
	in p2
	ld r2
	push r2
	
	; Configura endereco do tamanho do buffer em P0:P1
	cdr
	st stdsize::8
	out p0
	st stdsize::4
	shl 4
	st stdsize::0
	out p1
	
	; Ler o numero do tamanho
	in p2
	
	; Compara o tamanho com a posicao
	bt r2
	jz .noskip_read		; Se for igual, leia o buffer de teclado
	jc .skip_read		; Se for maior, nao leia o buffer de teclado
						; Leia o buffer de teclado se for menor ou igual
						
; Codigo acima e abaixo ate .skip_read e equivalente a: 
; if(stdpos <= stdsize) stdsize = readbuffer()
.noskip_read:
	; Carregue o buffer de teclado em stdin (incluindo 0x0A)
	; Ate encontrar um 0x0A, se nao houver tecla armazenada
	; Espere ate pressionar
	call readbuffer
	
	; Salve o tamanho de leitura em stdsize
	pushd
	cdr
	st stdsize::8
	out p0
	st stdsize::4
	shl 4
	st stdsize::0
	out p1
	popd
	out p2
	
; O codigo abaixo e equivalente a: 
; stdpos = (stdin[stdpos] == 0x0A) ? 0 : stdpos + 1;
.skip_read:
	; Configura os registradores iniciais pra comparacao
	cdr
	st $0A
	ld %r0
	pop r2	; Restaura stdpos
	
	; Configura o endereco do buffer [stdin + stdpos]
	cdr
	st stdin::8
	out p0
	st stdin::4
	shl 4
	st stdin::0
	add r2
	out p1
	
	; Leia o caractere do buffer
	in p2
	
	; Compara com 0x0A
	bt %r0
	jz .getc_line	; Se for igual, reseta stdpos
	jp .getc_incr	; Se for diferente, incrementa stdpos
	
; Reseta stdpos
.getc_line:
	pushd
	cdr
	st stdpos::8
	out p0
	st stdpos::4
	shl 4
	st stdpos::0
	out p1
	cdr
	out p2
	st stdsize::8
	out p0
	st stdsize::4
	shl 4
	st stdsize::0
	out p1
	cdr
	out p2
	jp .getc_done
	
; Incrementa stdpos
.getc_incr:
	pushd
	cdr
	st stdpos::8
	out p0
	st stdpos::4
	shl 4
	st stdpos::0
	out p1
	cdr
	st 1
	add r2
	out p2
	
; Retorna getchar com DR = caractere
.getc_done:
	popd
	popb
ret

stdin:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
stdpos:
	db 0
stdsize:
	db 0
	
readbuffer:
	cdr
	ld r0
	ld r2
	st $0A
	ld r3
	
.read:
	in p3
	bt r0
	jz .read

	out p3
	pushd
	cdr
	st stdin::8
	out p0
	st stdin::4
	shl 4
	st stdin::0
	add r2
	out p1
	popd
	
	out p2
	
	pushd
	cdr
	st 1
	add r2
	ld r2
	popd
	
	bt r3
	jz .read_done

	jp .read
	
.read_done:
	cdr
	or r2
	ret

; Putchar - Print the char in the TTY output
; In:
;	BP + 4 = Char to print
putchar:
	pushb
	pushs
	popb
	
	cdr
	st PARAM1
	abp
	out %p3
	
	popb
ret


