/*
byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';
byte func3() 0x1003 = 'C';

word funcs[3] = {&func1, &func2, &func3};
word i = 0;

while(1){
	(funcs[i % 3])();
	i = i + 1;
}
*/

word A = 0x1234;
if(1){
	word P = &A;
	*P = *P * 0x0002;
	*(P+1) = *(P+1) * 0x0002;
	if(*P == 0x2468){
		1;
	}else{
		2;
	}
}
