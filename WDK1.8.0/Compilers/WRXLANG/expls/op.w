include "../../../Libraries/SYS8/WRX/iosys.w"
	
word func(byte x){
	word A = 255;
	word B = 255;
	return A * B;
}

print("Valor: %w", func(2));

asm {"ED"}