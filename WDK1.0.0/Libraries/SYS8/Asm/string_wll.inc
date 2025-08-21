db "WLL",0

SymbolTable:
	dw strcmp_str, strcmp
	dw strcpy_str, strcpy
	dw strlen_str, strlen
	
strcmp_str:
	db "strcmp",0
	
strcpy_str:
	db "strcpy",0
	
strlen_str:
	db "strlen",0

strcmp:
	pushb
	pushs

	; TODO: Compare string1 with string2

	popb
ret

strcpy:
	pushb
	pushs

	; TODO: Copy from source str to dest str 

	popb
ret

strlen:
	pushb
	pushs

	; TODO: Count the ammount of string bytes

	popb
ret



