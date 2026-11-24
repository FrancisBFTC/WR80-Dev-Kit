include "..\..\..\Libraries\SYS8\WRX\iosys.w"

word i = 1;
i = i << 9;

print("Valor: %wu\n", i);

asm{"ED"}