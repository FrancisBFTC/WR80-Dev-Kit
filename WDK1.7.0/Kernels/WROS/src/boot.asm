define FILE_COUNT	5

ORG 0x000

jp Boot

include "boot.inc"

Boot:
	clr
	
.FileStart:
	pushd
	call set_table
	call next_file
	call set_str
	
.Loop:
	pushd
	cdr
	ld r2
	popd
	
	bt r2
	jz .Found

	cdr
	st 1
	ld r2
	popd
	add r2
	pushd
	
	in p2
	ld r2
	
	in p0
	pushd
	in p1
	pushd
	
	call update_table
	ld r3
	call incr_table
	
	popd
	out p1
	popd
	out p0
	call incr_str
	
	cdr
	or r3
	bt r2
	jz .Loop
	
	call read_count
	jc .NotFound
	
	popd
	cdr
	jp .FileStart
	
	

.Found:
	popd
	ld r2
	st 10
	sub r2

	call calc_diff
	call update_table
	pushd
	call incr_str
	pushd
	
	call set_table
	
	cdr
	st 2
	call calc_diff
	
	cdr
	st 0xF
	and r0
	ld r0
	out p0
	
	popd
	sub r0
	ld r0
	popd
	sub r1
	ld r1
	jc build_addr
	
	cdr
	st 1
	ld r2
	cdr
	or r0
	sub r2
	ld r0
	
build_addr:
	st 0xF
	shl 4
	or r0
	out p2
	
	call incr_str
	
	cdr
	or r1
	out p2
	
	jp table


ORG $0F2
table:
	db $0E,0
	
.NotFound:
	popd
	ec
	ret
	
ORG $0FE 
	dw $AA55
	