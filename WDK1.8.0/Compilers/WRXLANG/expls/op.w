include "../../../Libraries/SYS8/WRX/iosys.w"
	
word func(){
	word A = 10;
	word B = 2;
	return A / B;
}

print("Valor: %w", func());

asm {"ED"}