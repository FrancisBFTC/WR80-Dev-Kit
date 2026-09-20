include "..\..\..\Libraries\SYS8\WRX\iosys.w"
include "..\..\..\Libraries\SYS8\WRX\string.w"

word string = "ola pessoas";

if(compare(&string, "ola pessoas") == 0)
	print("E igual\n");
else
	print("E diferente\n");

if(compare(&string, "Oi pessoa") == 0)
	print("E igual\n");
else
	print("E diferente\n");
	
print("Tamanho: %u\n", length(&string));