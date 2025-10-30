define _r0 $40
define _r1 $49
define _r2 $4A
define _r3 $5B
define _r4 $64
define _r5 $6D
define _r6 $76
define _r7 $7F
define _ram 0x01

macro .times ...
	rep #.
		rep #-
			db #.
		endp
		rep #.
		endp
	endp
endm

macro .mov _reg1, _reg2
	stl #_reg2
	ld #_reg1
endm

macro .mov _reg1, _reg2, _label
	std #_label::8
	ld #_reg1
	std #_label::0
	ld #_reg2
endm

macro .movl _reg, _num
	std #_num
	ld #_reg
endm

macro .inb _reg, _port
	in #_port
	ld #_reg
endm

macro .inb _port
	in #_port
endm

macro .outb _port, _reg
	stl #_reg
	out #_port
endm

macro .outd _label
	std #_label::8
	out p0
	std #_label::0
	out p1
endm

macro .outd _port1, _port2, _label
	std #_label::8
	out #_port1
	std #_label::0
	out #_port2
endm

macro .inc _reg
	std #_reg
	idc
	incr
endm

macro .dec _reg
	std #_reg
	idc
	decr
endm

macro .clear _reg
	cdr
	ld #_reg
endm

macro .cmp _reg1, _reg2
	stl #_reg1
	bt #_reg2
endm

macro .cmpl _reg, _num
	push r0
	std #_num
	ld r0
	stl #_reg
	bt r0
	pop r0
endm

macro .jne _label
	jz end_##
	jp #_label
	end_##:
endm

macro .je _label
	jz #_label
endm

macro .jmp _label
	jp #_label
endm

macro .add _reg1, _reg2
	stl #_reg2
	add #_reg1
	ld #_reg1
endm

macro .addl _reg, _num
	std #_num
	add #_reg
	ld #_reg
endm

macro .sub _reg1, _reg2
	stl #_reg1
	sub #_reg2
	ld #_reg1
endm

macro .subl _reg, _num
	push r0
	std #_num
	ld r0
	stl #_reg
	sub r0
	ld #_reg
	pop r0
endm

macro .and _reg1, _reg2
	stl #_reg1
	and #_reg2
	ld #_reg1
endm

macro .andl _reg, _num
	std #_num
	and #_reg
	ld #_reg
endm

macro .shl _reg, _num
	stl #_reg
	shl #_num
	ld #_reg
endm

macro .shr _reg, _num
	stl #_reg
	shr #_num
	ld #_reg
endm

macro .mod _reg, _num
	push r0
	std #_num
	ld r0
	stl #_reg
	div r0
	stl r0
	ld #_reg
	pop r0
endm

macro .Invoke ...
	call #.
endm

macro .END
	jp $FFF
endm
