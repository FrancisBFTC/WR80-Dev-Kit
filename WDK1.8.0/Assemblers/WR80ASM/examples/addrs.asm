define ADDR_BASE  __start

jp __main

__start:
	jp function_1
	jp function_2
	jp function_3
	
function_1:
	ret
function_2:
	ret
function_3:
	ret
	
__main:
	call ADDR_BASE + 0
	call ADDR_BASE + 2
	call ADDR_BASE + 4

std 0x41
out p3
