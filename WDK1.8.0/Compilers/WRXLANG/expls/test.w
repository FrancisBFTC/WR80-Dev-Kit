byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';

word A = &func1;
word B = &func2;
word C = 0;
byte i = 0;

while(){
	word D = &A + (i % 4);
	C = *D;
	(*C)();
	i = i + 2;
}
