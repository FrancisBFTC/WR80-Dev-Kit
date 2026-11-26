byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';
byte func3() 0x1003 = 'C';

word funcs[3] = {&func1, &func2, &func3};
byte i = 0;

while(1){
	(funcs[i % 3])();
	i = i + 1;
}