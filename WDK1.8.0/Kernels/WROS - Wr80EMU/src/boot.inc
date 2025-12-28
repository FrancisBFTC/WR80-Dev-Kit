file:
	db "shell.bin",0
count:
	db 3

set_str:
	cdr
	st file::8
	out p0
	std file::0
	out p1
	in p2
ret

set_table:
	;std 0x10
	;ld r2

	;or r2
	cdr
	ld r2
	ld r0
	out p0
	std table::0
	ld r1
	out p1
ret

update_table:
	stl r0
	out p0
	stl r1
	out p1
	in p2
ret

incr_table:
	std 1
	add r1
	ld r1
	jc incr_high
	jp .done
incr_high:
	std 1
	add r0
	ld r0
.done:
	ret

incr_str:
	push r2
	std 1
	ld r2
	in p1
	add r2
	out p1
	jc incrs_high
	jp .dones
incrs_high:
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
	cdr
	st count::8
	out p0
	std count::0
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
	stl r1
	add r3
	ld r1
	jc addr_high
	jp .ret_next
addr_high:
	std 1
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
	std 1
	add r0
	ld r0
ret
