byte UNSIGNED 	= 0;
byte SIGNED		= 1;

byte print_char(byte c) 
	0x1003 = c;

byte print_hex(byte x, byte y){
	byte p;
	byte s = y;
	
	print_char('0');
	print_char('x');
	
	while(s){
		s = s - 4;
		p = x >> s & 0x0F;
		if(p < 10) print_char(p + '0'); else print_char(p - 10 + 'A');
	}
}

byte print_num(byte x, byte t){
	byte y = 100;
	if(x & 0x80 && t){
		x = -x;
		print_char('-');
	}
	while(y != 0){
		if(x >= y || !x)
			print_char(x / y + '0');
		x = x % y;
		y = y / 10;
	}
}


byte print_num_16(word x, byte t){
	word y = 10000;
	byte st = 0;
	if(x & 0x8000 && t){
		x = -x;
		print_char('-');
	}
	while(y != 0){
		if(x >= y || st || !x){
			byte c = x / y;
			c = c + '0';
			print_char(c);
			st = 1;
		}
		x = x % y;
		y = y / 10;
	}
}


byte puts(word A){
	while(*A){
		print_char(*A);
		A = A + 1;
	}
}

byte print(word char, byte args){
	byte j = 0;
	byte X;
	byte Y;
	while(*char){
		if(*char != '%'){
			if(*char != '\')
				print_char(*char);
			else{
				char = char + 1;
				if(*char == 'n')
					print_char(0x0A);
				else if(*char == 't')
					print_char(0x09);
				else{
					print_char(*char);
				}
				char = char + 1;
				continue;
			}
		}else{
			char = char + 1;
			byte arg = *(&args + j);
			if(*char == 'd')
				print_num(arg, SIGNED);
			else if(*char == 'u')
				print_num(arg, UNSIGNED);
			else if(*char == 'x')
				print_hex(arg, 8);
			else if(*char == 'a')
				print_char(arg);
			else if(*char == 'w'){
				word jw = j;
				word value = *(&args + jw);
				byte sign;
				j = j + 1;
				if(*(char+1) == 'u'){
					sign = UNSIGNED;
					char = char + 1;
				}else{
					sign = SIGNED;
				}
				print_num_16(value, sign);
			}
			char = char + 1;
			j = j + 1;
			continue;
		}
		char = char + 1;
	}
}