include "arit.asm"

define wand wsum & wsub
define wor  wmul | wdiv
define wxor wres1 ^ wor
define wshr wres1 >> wxor
define wshl wsub << wxor
define cond 0 && (0 || 1)
define cond2 wshr && wshl || cond

std wand
std wor
std wxor
std wshr
std wshl
std cond
std cond2

dw 0

