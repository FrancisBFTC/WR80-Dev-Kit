; aloca 02, 04, 06, 08
;macro .times ...
;	rep #*
;		db #.
;	endp
;endm

; aloca 02, 04, 06
;macro .times ...
;	rep #-
;		db #.
;	endp
;endm

; aloca 02, 04, 06, 08, 02
;macro .times ...
;	rep #+
;		db #.
;	endp
;endm

; cria 62, 64, 66, 68
;macro .times ...
;	rep #*
;		st #.
;	endp
;endm

; aloca 04, 06
macro .times ...
	rep #.
		db #.
	endp
endm

.times 02, 04, 06, 08
