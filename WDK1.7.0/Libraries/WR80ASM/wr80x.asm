define _r0 $40
define _r1 $49
define _r2 $4A
define _r3 $5B
define _r4 $64
define _r5 $6D
define _r6 $76
define _r7 $7F
define _ram 0x01
define .NULL 	0
define .NaN  	#$
define .N 		#*
define .N++		#+
define .N--		#-
define .I		#%
define .I++		#.

macro .times ...
	rep .I++
		rep .N--
			db .I++
		endp
		if .I++
		endf
	endp
endm

macro .mov _reg1, _reg2
	if DR
		if #_reg2 == DR
			ld #_reg1
		endf
		else
			if #_reg2 != .NaN
				std #_reg2
			endf
			else
				stl #_reg2
			ende
		ende
	endf
	else
		if BP && SP
			if #1 == BP
				.push #1
				.push #2
				.pop #1
			endf
			if #1 == SP
				.push #2
				.pop #1
				.pop #2
			endf
		endf
		else
			if BP || SP
				if #2 != .NaN
					std #2::8
					.push DR
					std #2::0
					.push DR
				endf
				else
					.push .NULL
					.push #2
				ende
				.pop #1
			endf
			else
				if #_reg2 != .NaN
					if #_reg2 == .NULL
						cdr
						ld #_reg1
					endf
					else
						std #_reg2
						ld #_reg1
					ende
				endf
				else
					stl #_reg2
					ld #_reg1
				ende
			ende
		ende
	ende
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
	if DR
		if #_reg2 == DR
			push r0
			ld r0
			stl #_reg1
			bt r0
			pop r0
		endf
		else
			if #_reg2 != .NaN
				push r0
				pushd
				std #_reg2
				ld r0
				popd
				bt r0
				pop r0
			endf
			else
				bt #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			if #_reg2 == .NULL
				if #1 != r0
					push r0
					cdr
					ld r0
					stl #_reg1
					bt r0
					pop r0
				endf
				else
					push r7
					cdr
					ld r7
					stl #_reg1
					bt r7
					pop r7
				ende
			endf
			else
				if #1 != r0
					push r0
					std #_reg2
					ld r0
					stl #_reg1
					bt r0
					pop r0
				endf
				else
					push r7
					std #_reg2
					ld r7
					stl #_reg1
					bt r7
					pop r7
				ende
			ende
		endf
		else
			stl #_reg1
			bt #_reg2
		ende
	ende
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
	if DR
		if #_reg2 == DR
			add #_reg1
			ld #_reg1
		endf
		else
			if #_reg2 != .NaN
				push r0
				pushd
				std #_reg2
				ld r0
				popd
				add r0
				pop r0
			endf
			else
				add #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			std #_reg2
			add #_reg1
			ld #_reg1
		endf
		else
			stl #_reg1
			add #_reg2
			ld #_reg1
		ende
	ende
endm

macro .addl _reg, _num
	std #_num
	add #_reg
	ld #_reg
endm

macro .sub _reg1, _reg2
	if DR
		if #_reg2 == DR
			push r0
			ld r0
			stl #_reg1
			sub r0
			ld #_reg1
			pop r0
		endf
		else
			if #_reg2 != .NaN
				if #1 != r0
					push r0
					std #_reg2
					ld r0
					sub r0
					pop r0
				endf
				else
					push r7
					std #_reg2
					ld r7
					sub r7
					pop r7
				ende
			endf
			else
				sub #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			push r0
			std #_reg2
			ld r0
			stl #_reg1
			sub r0
			ld #_reg1
			pop r0
		endf
		else
			stl #_reg1
			sub #_reg2
			ld #_reg1
		ende
	ende
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
	if DR
		if #_reg2 == DR
			and #_reg1
			ld #_reg1
		endf
		else
			if #_reg2 != .NaN
				push r0
				pushd
				std #_reg2
				ld r0
				popd
				and r0
				pop r0
			endf
			else
				and #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			std #_reg2
			and #_reg1
			ld #_reg1
		endf
		else
			stl #_reg1
			and #_reg2
			ld #_reg1
		ende
	ende
endm

macro .andl _reg, _num
	std #_num
	and #_reg
	ld #_reg
endm

macro .or _reg1, _reg2
	if DR
		if #_reg2 == DR
			or #_reg1
			ld #_reg1
		endf
		else
			if #_reg2 != .NaN
				push r0
				pushd
				std #_reg2
				ld r0
				popd
				or r0
				pop r0
			endf
			else
				or #_reg2
			ende
		ende
	endf
	else
		if #_reg2 != .NaN
			std #_reg2
			or #_reg1
			ld #_reg1
		endf
		else
			stl #_reg1
			or #_reg2
			ld #_reg1
		ende
	ende
endm

macro .orl _reg, _num
	std #_num
	or #_reg
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

macro .push _num
	if #1 == .NaN
		if #_num == DR
			pushd
		endf
		else
			if #_num == BP
				pushb
			endf
			else
				if #_num == SP
					pushs
				endf
				else
					push #_num
				ende
			ende
		ende
	endf
	else
		std #_num
		pushd
	ende
endm

macro .pop _num
	if #_num == .NaN
		if #_num == DR
			popd
		endf
		else
			if #_num == BP
				popb
			endf
			else
				if #_num == SP
					pops
				endf
				else
					pop #_num
				ende
			ende
		ende
	endf
	else
		std #_num
		abp
	ende
endm

macro .ret ...
	if .N > 1
		std #1
	endf
	ret
endm

macro .Invoke ...
	if .N > 1
		if .I++
		endf
		.mov BP, SP
		rep .N--
			.push .I++
		endp
		.Invoke .I
		.mov SP, BP
	endf
	else
		call .I
	ende
endm

macro .END
	jp $FFF
endm
