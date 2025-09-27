file:
	db "shell.bin",0
count:
	db 3

set_str:
	cdr	
	st file::8
	out p0
	st file::4
	shl 4
	st file::0
	out p1
	in p2
ret

set_table:	
	cdr
	st table::8
	ld r0
	out p0
	st table::4
	shl 4
	st table::0
	ld r1
	out p1
ret

update_table:
	cdr
	or r0
	out p0
	cdr
	or r1
	out p1
	in p2
ret

incr_table:
	cdr
	st 1
	add r1
	ld r1
	jc incr_high
	jp .done
incr_high:
	cdr
	st 1
	add r0
	ld r0
.done:
	ret

incr_str:
	push r2
	cdr
	st 1
	ld r2
	in p1
	add r2
	out p1
	jc incrs_high
	jp .dones
incrs_high:
	cdr
	in p0
	add r2
	out p0
.dones:
	pop r2
	in p2
ret

read_count:
	in p0
	pushd
	in p1
	pushd
	
	cdr
	ld r0
	st 1
	ld r1
	st count::8
	out p0
	st count::4
	shl 4
	st count::0
	out p1
	
	in p2
	sub r1
	out p2
	bt r0
	
	popd
	out p1
	popd
	out p0
	
	jz .error
	
	dc
	ret
	
.error:
	ec
ret

next_file:
	in p2
	ld r3
	cdr
	or r1
	add r3
	ld r1
	jc addr_high
	jp .ret_next
addr_high:
	cdr
	st 1
	add r0
	ld r0
.ret_next:
	std 14
	add r3
	out p2
ret

calc_diff:
	add r1
	ld r1
	jc add_r0
	ret
add_r0:
	cdr
	st 1
	add r0
	ld r0
ret
