ORG 0x100
	
Label:
	st Label::8
	ld r0
	st Label::4
	shl 4
	st Label::0
	ld r1
	
; R0:R1 = Endereco de Label -> 0x100
	
	