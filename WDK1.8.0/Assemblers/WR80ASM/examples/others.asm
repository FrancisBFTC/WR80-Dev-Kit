define .head 10 / 5					; .head = 2
define .size addr_end - addr_start	; .size = 8

org .head

addr_start:
	db "AAAAAAAA"
addr_end:

addr_size:	
	dw .size
