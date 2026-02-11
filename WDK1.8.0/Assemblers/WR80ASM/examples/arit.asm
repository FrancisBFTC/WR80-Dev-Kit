define wsum		3 + 4
define wsub 	7 - 3
define wsub2	3 - 7
define wmul		3 * 2
define wdiv		6 / 2
define wres1 	((wsum + wsub) - (wsub2 + wmul)) / wdiv * 2
define wres2	(((3 + 4) + (7 - 3)) - ((3 - 7) + (3 * 2))) / (6 / 2) * 2

std wsum
std wsub
std wsub2
std wmul
std wdiv
std wres1
std wres2 % wsub

dw 0
