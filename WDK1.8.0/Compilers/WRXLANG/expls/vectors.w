include "..\..\..\Libraries\SYS8\WRX\iosys.w"

byte bytes[] = {16, 32, 64};
word words[] = {1024, 2048, 4096};

byte num_byte = bytes[0];
word num_word = words[1];
print(" BYTE: %u \n WORD: %wu \n", num_byte, num_word);

asm{"ED"}