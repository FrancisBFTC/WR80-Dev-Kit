/*
byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';
byte func3() 0x1003 = 'C';

word B = &func2;
word C = &func3;
word X = 0;
byte i = 0;

while(){
	word D = &A + (i % 6);
	X = *D;
	(*X)();
	i = i + 2;
}
*/

include "../../../Libraries/SYS8/WRX/iosys.w"
include "../../../Libraries/SYS8/WRX/math.w"

byte i = 5;
while(i > 0){
	print("Fatorial de %d: %d\n", i, factorial(i));
	print("Primorial de %d: %d\n", i, primorial(i));
	print("Termial de %d: %d\n\n", i, termial(i));
	
	byte f = factorial(i);
	byte p = primorial(i);
	byte t = termial(i);
	
	if(f == p && p == t)
		print("%d e um numero FPT\n\n", i);
	i = i - 1;
}
