word memory = "20000";
word mem = &memory;

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
			if(val == '0'){
				while(c != ']'){
					code = code + 1;
					c = *code;
				}
				code = code + 1;
				c = *code;
				continue;
			}else{
		
			}
		}else{
			return 0;
		}
		code = code + 1;
		c = *code;
	}
	return 1;
}