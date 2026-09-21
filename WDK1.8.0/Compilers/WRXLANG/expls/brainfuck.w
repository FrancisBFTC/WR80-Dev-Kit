word mem = 0xE00;
0xE00 = 0;
0xE01 = 0;
0xE02 = 0x30;

byte BrainFuck(word code){
	byte c = *code;
	byte dept = 0;
	while(c){
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
			dept = dept + 1;
			if(!*mem){
				while(c != ']' && dept){
					code = code + 1;
					c = *code;
					if(c == '[')	dept = dept + 1;
					if(c == ']')	dept = dept - 1;
				}
			}else{
				word temp = code;
				BrainFuck(code + 1);
				code = temp;
				c = *code;
				continue;
			}
		}else if(c == ']'){
			if(!dept){
				return 0;
			}
			dept = dept - 1;
			return 1;
		}else{
			return 0;
		}
		code = code + 1;
		c = *code;
	}
	return 1;
}