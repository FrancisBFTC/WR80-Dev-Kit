/*
include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 255;
word B = 255;
word str = "Hello World!";
word intro = "Testando Parsing de WORDs!";

print("\n %s \n*** %s ***\n\n", &str, &intro);
print(" Resultado : %wu\n", A * B);
*/

include "../../../Libraries/SYS8/WRX/iosys.w"

byte A = 0x80;
byte C = A >> 7;
print("Val: %u\n", C);
asm{"ED"}
