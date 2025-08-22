; Define o bit<4> de P0 para endereçamento da ROM
ST 1
SHL 4
OUT P0

; P0:P1 aponta o 1ª endereço da ROM
; Carrega caractere da ROM para a RAM
Load:
	IN P2
	LD R2
	AND R0
	OUT P0
	ST 14
	OUT P1
	AND R0
	OR R2
	OUT P2
	JP 0xFFF
