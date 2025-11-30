Start:
	ed
	st table::8
	shl 4
	st table::0
	out p1
	dd
	ei

end:
	jp end

Table:
	dw Int1, Handler, Handler, Handler
	

Int1:
	ed
	di
	
	st 1
	
	ei
	dd
iret

Handler:
	di
	ei
iret
