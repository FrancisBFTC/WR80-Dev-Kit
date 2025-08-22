ORG 0x0000
	JP Main

	Valor1:
		db "Numero 1: ",0
	Valor2: 
		db "Numero 2: ",0

Main:
	; Endereço P0:P1 = 0x0:0x02
	ST Valor1:8		; DR = Endereço de Valor1 -> 0x00
	OUT P0			; P0 = DR -> 0x00
	ST Valor1:0		; DR = Endereço de Valor1 -> 0x02
	OUT P1			; P1 = DR -> 0x02

	; Inicializa registro com 1
	ST 1			; DR = 1
	LD R1			; Carregue de DR para R1

ReadString1:
	; Ler o byte do endereço e compara com nulo
	IN P2				; Carrega o byte de P0:P1 para DR
	BT R0				; Compara DR com 0
	JZ FinalString1 	; Se for 0, Salte para a label FinalString1

WriteMonitor1:
	OUT P3 				; Escreve no monitor o que está em DR

AddAddress1:
	; Soma +1 o endereço de memória
	IN P1				; DR = P1 -> 0x02
	ADD R1 				; DR = DR + R1
	OUT P1				; P1 = DR -> 0x03
	JP ReadString1

FinalString1:
	; Realiza o Parse do número 9 -> '9'
	ST 0x03				; DR = 3
	SHL 4				; DR = DR << 4 = 0x30
	ST 9				; DR<3-0> = 9
	OUT P3 				; P3 = DR = 9
	AND R0

	; Endereço P0:P1 = 0x0:0x02
	ST Valor2:8		; DR = Endereço de Valor1 -> 0x00
	OUT P0			; P0 = DR -> 0x00
	ST Valor2:0		; DR = Endereço de Valor1 -> 0x02
	OUT P1			; P1 = DR -> 0x02

	; Inicializa registro com 1
	ST 1			; DR = 1
	LD R1			; Carregue de DR para R1

ReadString2:
	; Ler o byte do endereço e compara com nulo
	IN P2				; Carrega o byte de P0:P1 para DR
	BT R0				; Compara DR com 0
	JZ FinalString2 	; Se for 0, Salte para a label FinalString1

WriteMonitor2:
	OUT P3 				; Escreve no monitor o que está em DR

AddAddress2:
	; Soma +1 o endereço de memória
	IN P1				; DR = P1 -> 0x02
	ADD R1 				; DR = DR + R1
	OUT P1				; P1 = DR -> 0x03
	JP ReadString2

FinalString2:
	; Realiza o Parse do número 9 -> '9'
	ST 0x03				; DR = 3
	SHL 4				; DR = DR << 4 = 0x30
	ST 7				; DR<3-0> = 9
	OUT P3 				; P3 = DR = 9

END:
	JP END
