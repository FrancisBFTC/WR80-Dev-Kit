word memory = "012000000";
word mem = &memory;

byte bf_run(word code){
	while(*code != 0){
		if(*code == '>'){
			mem = mem + 1;
		}else if(*code == '<'){
			mem = mem - 1;
		}else if(*code == '.'){
			0x1003 = *mem;
		}
		else if(*code == '+'){
			*mem = *mem + 1;
		}
		else if(*code == '-'){
			*mem = *mem - 1;
		}
		
		code = code + 1;
	}
}

word str = ".++.>.++.>";
bf_run(&str);



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