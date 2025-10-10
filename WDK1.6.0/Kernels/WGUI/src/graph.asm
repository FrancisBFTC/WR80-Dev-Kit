jp Main		; Salta para o inicio do programa

include "thread.asm"

; Pinta o fundo e configura as tasks
Main:
	call SetStr
	call Print
	call DrawBackGround

; Inicia as tarefas simultaneas
	jp StartTasks
	jp END

; Processo 1 da primeira tarefa
Process1:
	ei
	pushs		; salva SP na pilha
	popb		; Restaura SP em BP, BP = SP
	std 0x20	; Configura valor de alocacao minima
	ssp			; Aloca 32 bytes minimos (para espacos internos do timer)
	
	std WIDTH
	ld r1		; R1 = 120
	std HEIGHT
	ld r2
	std POSX
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	call DrawWindow
	
	std GREEN
	ld r6
.wait_msg1:
	std _titlew_1::8
	out p0
	std _titlew_1::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg1
.ret_proc1:
	pushb
	pops
	di
ret

; Processo 2 da segunda tarefa
Process2:
	ei
	pushs		; salva SP na pilha
	popb		; Restaura SP em BP, BP = SP
	std 0x20	; Configura valor de alocacao minima
	ssp			; Aloca 32 bytes minimos (para espacos internos do timer)
	
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std 132
	ld r4
	std POSY
	ld r5
	std WHITE
	ld r6
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	call DrawWindow
	
	std BLUE
	ld r6
.wait_msg2:
	std _titlew_2::8
	out p0
	std _titlew_2::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg2
.ret_proc2:
	pushb
	pops
	di
ret

; Processo 3 da terceira tarefa
Process3:
	ei
	pushs		; salva SP na pilha
	popb		; Restaura SP em BP, BP = SP
	std 0x20	; Configura valor de alocacao minima
	ssp			; Aloca 32 bytes minimos (para espacos internos do timer)
	
	std WIDTH
	ld r1
	std HEIGHT
	ld r2
	std POSX
	ld r4
	std 52
	ld r5
	std WHITE
	ld r6
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	call DrawWindow
	
	std WHITE
	ld r6
	
.wait_msg3:
	std _titlew_3::8
	out p0
	std _titlew_3::0
	out p1
	push r4
	std _R6
	ld r7
	idc
	incr
	call PrintString
	pop r4
	jp .wait_msg3
.ret_proc3:
	pushb
	pops
	di
ret

include "graphdat.asm"
include "graphlib.asm"
include "strlib.asm"
;include "font5x5.asm"
include "font8x8.asm"

END:
	
