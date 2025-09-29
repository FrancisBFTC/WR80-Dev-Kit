Begin:
	clr
	call ConfigTimer
	call ConfigKeyboard
	
	std IntTable::8
	out p0
	std IntTable::0
	out p1
	ei

	
.lock:
	jp .lock
	

IntTable:
	dw Keyboard
	dw Mouse
	dw Timer
	dw Unknown

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
	std 0x41
	out p3
	ei
iret
; ------------------

; ISR 3 ------------
Unknown:

iret
; ------------------

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
	std 0x02		; Ler dispositivo no controller
	out p7
	call WaitACK
	std 0x02		; Escolher Dispositivo Timer -> Command: 0x02, Data: 0x02
	out p7
	call WaitACK
	
	; Configurar contador de 32 bits -> Valor: MAX
	; Velocidade mínima = ~125ms (0xFFFFFFFF)
	; Velocidade máxima = ? (0x2000FF)
	std $FF
	out p7
	call WaitACK
	std $FF
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

