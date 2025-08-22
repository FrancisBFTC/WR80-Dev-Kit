db "WLL",0

SymbolTable:
	dw strcmp_str, strcmp
	dw strcpy_str, strcpy
	dw strncpy_str, strncpy
	dw strlen_str, strlen
	
strcmp_str:
	db "strcmp",0
	
strcpy_str:
	db "strcpy",0
	
strncpy_str:
	db "strncpy",0
	
strlen_str:
	db "strlen",0

define PARAM1 4
define PARAM2 5
define PARAM3 6
define PARAM4 7
define PARAM5 8

; strcmp - Compare two strings
; In:
;	BP + 4 = High Part address string 1
;	BP + 5 = Low Part address string 1
;	BP + 6 = High Part address string 2
;	BP + 7 = Low Part address string 2
; Out:
;	DR = 0 if equal, otherwise, DR != 0
strcmp:
	pushb
	pushs
	popb

	cdr
	st PARAM1
	abp
	ld r0
	
	cdr
	st PARAM2
	abp
	ld r1
	
	cdr
	st PARAM3
	abp
	ld r2
	
	cdr
	st PARAM4
	abp
	ld r3
	
	push r3
	push r2
	push r1
	push r0
	
cmp_begin:
	cdr
	ld r0
	
cmp_str:
	popd
	out p0
	popd
	out p1
	in p2
	ld r1
	
	popd
	out p0
	popd
	out p1
	in p2
	
	bt r1
	jz inc_addr
	jp cmp.end
	
inc_addr:
	bt r0
	jz cmp.end
	
	cdr
	st 1
	ld r0
	ssp
	popd
	add r0
	pushd
	
	cdr
	st 2
	ssp
	popd
	add r0
	pushd
	
	cdr
	st 1
	ssp
	jp cmp_begin
	
cmp.end:
	xor r1
	popb
ret

; strcpy - Copy from source str to dest str 
; In: 
;	BP + 4 = High Part source string
;	BP + 5 = Low Part source string
;	BP + 6 = High Part destiny buffer string
;	BP + 7 = Low Part destiny buffer string
; Out:
;	R0:DR = address of destiny buffer
strcpy:
	pushb
	pushs
	popb

	cdr
	st PARAM1
	abp
	ld r0
	
	cdr
	st PARAM2
	abp
	ld r1
	
	cdr
	st PARAM3
	abp
	ld r2
	
	cdr
	st PARAM4
	abp
	ld r3
	
	push r3
	push r2
	
	push r3
	push r2
	push r1
	push r0
	
cpy_begin:
	cdr
	ld r0

cpy_str:
	popd
	out p0
	popd
	out p1
	in p2
	ld r1
	
	popd
	out p0
	popd
	out p1
	
	cdr
	or r1
	out p2
	
	bt r0
	jz cpy.end

	cdr
	st 1
	ld r0
	ssp
	popd
	add r0
	pushd
	jc cpy.h1
	
	cdr
	st 2
	jp skip_cpy1
	
cpy.h1:
	cdr
	st 1
	ssp
	popd
	add r0
	pushd
	
	cdr
	st 1
	
skip_cpy1:
	ssp
	popd
	add r0
	pushd
	jc cpy.h2
	jp skip_cpy2
	
cpy.h2:
	cdr
	st 1
	ssp
	popd
	add r0
	pushd
	jp cpy_begin
	
skip_cpy2:
	cdr
	st 1
	ssp
	jp cpy_begin
	
cpy.end:
	pop r0
	popd
	popb
ret

; strncpy - Copy N bytes from source str to dest str 
; In:
;	BP + 4 = N bytes to copy
;	BP + 5 = High Part source string
;	BP + 6 = Low Part source string
;	BP + 7 = High Part destiny buffer string
;	BP + 8 = Low Part destiny buffer string
; Out:
;	R0:DR = address of destiny buffer
strncpy:
	pushb
	pushs
	popb

	cdr
	st PARAM1
	abp
	pushd
	
	cdr
	st PARAM2
	abp
	ld r0
	
	cdr
	st PARAM3
	abp
	ld r1
	
	cdr
	st PARAM4
	abp
	ld r2
	
	cdr
	st PARAM5
	abp
	ld r3
	popd
	
	push r3
	push r2
	
	push r3
	push r2
	push r1
	push r0
	pushd
	
ncpy_begin:
	pop r3
	st 1
	ld r2
	
ncpy_str:
	popd
	out p0
	popd
	out p1
	in p2
	ld r1
	
	popd
	out p0
	popd
	out p1
	
	cdr
	or r1
	out p2
	
	cdr
	or r3
	sub r2
	ld r3
	jz ncpy.end

	cdr
	st 1
	ld r0
	ssp
	popd
	add r0
	pushd
	jc ncpy.h1
	
	cdr
	st 2
	jp skip_ncpy1
	
ncpy.h1:
	cdr
	st 1
	ssp
	popd
	add r0
	pushd
	cdr
	st 1
	
skip_ncpy1:
	ssp
	popd
	add r0
	pushd
	jc ncpy.h2
	
	cdr
	st 1
	jp skip_ncpy2
	
ncpy.h2:
	cdr
	st 1
	ssp
	popd
	add r0
	pushd
	
skip_ncpy2:
	ssp
	push r3
	jp ncpy_begin
	

ncpy.end:
	pop r0
	popd
	popb
ret

strlen:
	pushb
	pushs
	popb

	; TODO: Count the ammount of string bytes

	popb
ret
