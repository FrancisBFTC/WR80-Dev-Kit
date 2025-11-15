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

	std 1
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
	
	stl r3
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
	std 10
	sub r2

	call calc_diff
	call update_table
	pushd
	call incr_str
	pushd
	
	call set_table
	
	std 2
	call calc_diff
	
	std 0xF
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
	
	std 1
	ld r2
	stl r0
	sub r2
	ld r0
	
build_addr:
	std $F0
	or r0
	out p2
	
	call incr_str
	
	stl r1
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
	