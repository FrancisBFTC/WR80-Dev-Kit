word mem = 0xE00;
0xE00 = 0x30;

byte bf_run(word code){
	byte c = *code;
	byte val = 0;
	while(c){
		val = *mem;
		if(c == '>'){
			mem = mem + 1;
		}else if(c == '<'){
			mem = mem - 1;
		}else if(c == '.'){
			0x1003 = *mem;
		}else if(c == '+'){
			*mem = *mem + 1;
		}else if(c == '-'){
			*mem = *mem - 1;
		}else if(c == '['){
			if(val == 0){
				while(c != ']'){
					code = code + 1;
					c = *code;
				}
			}else{
				word temp = code;
				code = code + 1;
				bf_run(code);
				code = temp;
				continue;
			}
		}else if(c == ']'){
			*mem = *mem - 1;
			return 1;
		}else{
			return 0;
		}
		code = code + 1;
		c = *code;
	}
	return 1;
}