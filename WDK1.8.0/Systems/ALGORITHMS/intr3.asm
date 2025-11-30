Start:
	st Table::8
	shl 4
	st Table::0
	out p1
	ei

end:
	jp end

;org 0x20
Table:
	dw Int1, Int2, Int3, Int4

;org 0x40
Int1:
	di
	st 1
	call Addr
	ei
iret

db 0
Addr:
	st 15
ret

;org 0x50
Int2:
	di
	st 2
	ei
iret

;org 0x60
Int3:
	di
	st 3
	ei
iret

;org 0x70
Int4:
	di
	st 4
	ei
iret
