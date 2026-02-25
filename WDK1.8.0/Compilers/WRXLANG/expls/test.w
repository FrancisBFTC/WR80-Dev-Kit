/*
word buffer = "000000000";

byte bf_run(word code, word memory){
	byte i = 0;
	byte j = 0;
	word char = &code;
	word mem = &memory;
	while(*char){
		if(*char == '>'){
			//j = j + 1;
			mem = &memory + 1;
		}else if(*char == '<'){
			//j = j - 1;
			mem = &memory + 1;
		}else if(*char == '.'){
			0x1003 = *mem;
		}
		else if(*char == '+'){
			byte val = *mem + 1;
			0x1003 = val;
			*mem = val;
		}
		else if(*char == '-'){
			1;
		}
		
		i = i + 1;
		char = &code + i;
	}
}

bf_run("+.+.", &buffer);

*/


byte func1() 0x1003 = 'A';
byte func2() 0x1003 = 'B';
byte func3() 0x1003 = 'C';

word A = &func1;
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
