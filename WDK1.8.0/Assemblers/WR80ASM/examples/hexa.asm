include "..\..\WR80-Dev-Kit\WDK1.8.0\Libraries\WR80ASM\wr80x.asm"

define BIT_0 	1 << 0
define BIT_1 	1 << 1
define BIT_2 	1 << 2
define BIT_3 	1 << 3
define ISOL	 	BIT_3 | BIT_2 | BIT_1 | BIT_0
define PART_12	ISOL << 8
define PART_8	ISOL << 4
define PART_4	ISOL << 0
define ADDR 	0x123-1
define WEXP		2 ** 4

;.times 	ADDR, 0xFF

org ADDR
_start:
	st (_start & PART_12) >> 8
	st (_start & PART_8) >> 4
	st (_start & PART_4) >> 0
	
	st (123h & h'F00') >> 8
	st (123h & h'0F0') >> 4
	st (123h & h'00F') >> 0
	std WEXP
	