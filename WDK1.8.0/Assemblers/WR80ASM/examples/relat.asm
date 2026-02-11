include "logic.asm"

define gt 	wshr > wshl
define lt 	wshr < wshl
define gte 	wsum >= wor
define gt2  wsum < 7
define lte 	wor <= 8
define equ 	wsum == wor
define dif	wsum != wshl

std gt
std lt
std gte
std gt2
std lte
std equ
std dif

dw 0
