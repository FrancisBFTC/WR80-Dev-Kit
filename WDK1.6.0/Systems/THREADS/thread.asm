Begin:
	clr
	call ConfigTimer
	;call ConfigKeyboard
	
	std $0D
	ld r1
	
	std IntTable::8
	out p0
	std IntTable::0
	out p1
	ei
	
.lock:
	std $41
	out p3
	jp .lock
	

IntTable:
	dw Keyboard
	dw Mouse
	dw Timer
	dw Unknown
	
define PID_1 0x0001
define PID_2 0x0002
define PID_3 0x0003
define PID_END 0x0204

ProcTable:
	dw Process1,  PID_1
	dw Process2,  PID_2
	dw Process3,  PID_3
	dw 0x0000,    PID_END
	
AddrCall:
	dw 0x0000
	
ProcIndex:
	db $FF
Status:
	db 0

; ISR 0 ------------
Keyboard:
	di
	std 0x02
	out p7
	call WaitACK
	std 0x00
	out p7
	call WaitACK
	in p7
	out p3
	ei
iret
; ------------------

; ISR 1 ------------
Mouse:

iret
; ------------------

; ISR 2 ------------
Timer:
	di
	pushs
	pushb
	pushd
	in p0
	pushd
	in p1
	pushd
	in p4
	pushd
	in p5
	pushd
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	
	cdr
	ld r3
	
.NextProc:
	pushs
	pop r6
	pop r5
	std 0x50
	ssp
	push r5
	push r6
	call ReadProc
	pops
	
	; R4 = PID, DR = Status
	; Verificar Status 0, 1 ou 2
	ld r7
	cdr
	bt r7
	jz .ProcessBegin	; Se status = 0, Inicia processo
	
	std $5B
	idc
	incr			; Incrementa R3
	std 0x03
	bt r7
	jz .NextProc
	
	std 0x41
	idc
	cdr
	std .ProcessCall::8	; Aqui retorna std 0
	ld r0
	std .ProcessCall::0
	ld r1
	incr
	push r0
	push r1
	popd
	popd
	
	std PID_END::0
	bt r4
	jz .StackReset		; Se status = 3, Fim da lista de Processos
	jp .SkipStackReset	; Se status = 1 | 2, Processo em execução
	
.StackReset:
	std ProcIndex::8
	out p0
	std ProcIndex::0
	out p1
	std $0F
	pushd
	std $FF
	pushd
	pops
	out p2
	
	std 19
	ssp
	jp .RestoreContext
	
.SkipStackReset:
	std 21
	mul r3
	ssp
	jp .RestoreContext
	
.ProcessBegin:
	pushs
	pop r6
	pop r5
	std 0x50
	ssp
	push r5
	push r6
	call CalcAddr
	pops
	clr
	
.ProcessCall:
	call AddrCall
	
	std 0x80
	idc
	std ProcIndex::8
	out p0
	std ProcIndex::0
	out p1
	in p2
	decr
	out p2
	
	call ReadProc
	
	std 0x03
	out p2
	
	cdr
	pushd
	pushd
	ei
.lock1:
	jp .lock1
	
	; Alterar processo para status 2 (finalizado)
	; Atribuir BP = SP e Subtrair BP - 25 e BP - 24 -> (Process2 Int Return)
	; Ler Endereço de Int. de Process2
	; Subtrair SP - 2 -> Com PUSHD, PUSHD vai para Process1 Int Return
	; E salvar Process2 Int com PUSHD, PUSHD em Process1 Int
	; Subtrair SP - 17 e SP - 21 para restaurar contexto de Process2
	; Restaurar contexto de Process2
	; Adicionar SP + 4 e SP + 17
	; Retornar com IRET (Alterna para Process2)
.RestoreContext:
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	popd
	out p5
	popd
	out p4
	popd
	out p1
	popd
	out p0
	stl r7
	idc
	popd
	popb
	pops
	ei
iret
; ------------------

; ISR 3 ------------
Unknown:

iret
; ------------------

Process1:
	ei
	std $42
	ld r2
	cdr
	ld r0
	std $31
	ld r1
	.loop_proc1:
		stl r2
		out p3
		in p3
		bt r0
		jz .loop_proc1
		bt r1
		jz .ret_proc1
		jp .loop_proc1
.ret_proc1:
	di
ret

Process2:
	ei
	std $43
	ld r2
	cdr
	ld r0
	std $32
	ld r1
	.loop_proc2:
		stl r2
		out p3
		in p3
		bt r0
		jz .loop_proc2
		bt r1
		jz .ret_proc2
		jp .loop_proc2
.ret_proc2:
	di
ret

Process3:
	ei
	std $44
	ld r2
	cdr
	ld r0
	std $33
	ld r1
	.loop_proc3:
		stl r2
		out p3
		in p3
		bt r0
		jz .loop_proc3
		bt r1
		jz .ret_proc3
		jp .loop_proc3
.ret_proc3:
	di
ret


ConfigTimer:
	;ed
	std $FA			; ACKnowledge Response
	ld r0
	std 0x01		; Abrir dispositivo no controller
	out p7
	call WaitACK
	std 0x02		; Escolher Dispositivo Timer -> Command: 0x01, Data: 0x02
	out p7
	call WaitACK
	call WriteTimerLimit
ret

WriteTimerLimit:
	std 0x02		; Ler dispositivo no controller
	out p7
	call WaitACK
	std 0x02		; Escolher Dispositivo Timer -> Command: 0x02, Data: 0x02
	out p7
	call WaitACK
	
	; Configurar contador de 32 bits -> Valor: MAX
	; Velocidade mínima = ~125ms (0xFFFFFFFF)
	; Velocidade máxima = ? (0x2000FF)
	std $00
	out p7
	call WaitACK
	std $20
	out p7
	call WaitACK
	std $FF
	out p7
	call WaitACK
	std $FF
	out p7
	call WaitACK
ret

WaitACK:
	in p7
	bt r0
	jz ACK.Done
	jp WaitACK
ACK.Done:
	ret
	
ConfigKeyboard:
	std $FA			; ACKnowledge Response
	ld r0
	std 0x01
	out p7
	call WaitACK
	std 0x00
	out p7
	call WaitACK
ret

; Input  -> DR : Vector Index
; Output -> R0:R1 : Absolute Address
; 		 -> DR : Program Status = 0,1 or 2
;		 -> R4 : Program PID
ReadProc:
	std 0x80
	idc
	std ProcIndex::8
	out p0
	std ProcIndex::0
	out p1
	in p2
	incr
	out p2
	
	pushd
	std ProcTable::8
	ld r0
	std ProcTable::0
	ld r1
	popd
	
	shl 2
	add r1
	ld r1
	jc .incr0
	jp .defports
.incr0:
	std 0x40
	idc
	incr
.defports:
	stl r0
	out p0
	stl r1
	out p1
	
	std 0x01
	idc
	in p2
	ld r1
	incr
	in p2
	ld r0
	incr
	in p2
	ld r4
	incr
	in p2
	pushd
	ld r7
	cdr
	bt r7
	jz .updstatus
	jp .retread
.updstatus:
	std 1		; Altera status para execução -> 1
	out p2
.retread:
	popd
ret

; Input  -> R0:R1 : Absolute Address
;        -> R2:R3 : Jump Address
; Output -> R0:R1 : Jump + Relative Address
CalcAddr:
	pushd
	std AddrCall::8
	ld r2
	out p0
	std AddrCall::0
	ld r3
	out p1
	
	std 2
	add r3
	ld r3
	jc .incr2
	jp .calcrel
.incr2:
	std 0x52
	idc
	incr

.calcrel:
	stl r0
	sub r2
	ld r0
	stl r1
	sub r3
	ld r3
	jc .skipinc
	std 0x40
	idc
	decr
.skipinc:
	std $F0
	or r0
	ld r0
	stl r3
	ld r1
	
	std 0x01
	idc
	stl r0
	out p2
	incr
	stl r1
	out p2
	popd
ret

.End:

