Main:
	shl 4
	shr 4
	shl 4
	std 2
	ld %r1
	std 5
	div %r1
	
	ld %r1
	std 0x30
	or %r1
	out %p3
	
	std 10
	ld %r1
	stl %r0
	mul %r1
	
	ld %r0
	std 2
	ld %r1
	stl %r0
	div %r1
	
	ld %r1
	std $2E
	out %p3
	
	std 0x30
	or %r1
	out %p3
END:
	
	