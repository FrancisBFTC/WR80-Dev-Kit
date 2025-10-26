define _R1 0x49
define _P1 0x09

macro .inc _reg1
	std #_reg1
	idc
	incr
endm

.inc _R1
.inc _P1
	