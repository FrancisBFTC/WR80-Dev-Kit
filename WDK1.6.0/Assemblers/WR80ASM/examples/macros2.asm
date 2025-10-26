define .R1 0x49

macro _inc reg1
	std #reg1
	idc
	incr
endm

_inc .R1

