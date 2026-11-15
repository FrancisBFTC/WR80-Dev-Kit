include "../../../Libraries/SYS8/WRX/iosys.w"

/*
word func(){
	word A = 255;
	word B = 255;
	return A * B;
}

print("Valor: %wu", func());

asm {"ED"}
*/


word A = 255;
word B = 255;
print("Valor : %wu", A * B);
asm{"ED"}
