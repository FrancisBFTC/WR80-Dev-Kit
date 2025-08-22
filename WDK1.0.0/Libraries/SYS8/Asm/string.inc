define PARAM1 4
define PARAM2 5
define PARAM3 6
define PARAM4 7

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

strcpy:
	pushb
	pushs
	popb

	; TODO: Copy from source str to dest str 

	popb
ret

strlen:
	pushb
	pushs
	popb

	; TODO: Count the ammount of string bytes

	popb
ret



