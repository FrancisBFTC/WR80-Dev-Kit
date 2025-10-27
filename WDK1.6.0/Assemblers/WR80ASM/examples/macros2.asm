define _R1 0x49		; definicao
define _P1 0x09		; definicao

macro .inc _reg1
	std #_reg1
	idc
	incr
endm

.inc _R1
.inc _P1
	