macro .jne _label
	jz end##
	jp #_label
	end##:
endm

.jne code
.jne code
.jne code
.jne code
.jne code

code:
	and r0