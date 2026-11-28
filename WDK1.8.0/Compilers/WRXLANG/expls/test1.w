/*
byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';
byte func3() 0x1003 = 'C';

word funcs[3] = {&func1, &func2, &func3};
byte i = 0;

while(1){
	(funcs[i % 3])();
	i = i + 1;
}
*/

include "..\..\..\Libraries\SYS8\WRX\iosys.w"

if(1){
	byte numeros[3] = {10, 20, 30};
	print("NUM 0: %d\n", numeros[0]);
	print("NUM 1: %d\n", numeros[1]);
	print("NUM 2: %d\n", numeros[2]);
}