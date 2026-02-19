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

byte puts(word A){
	byte i = 0;
	word B = &A + i;
	while(*B){
		print_char(*B);
		i = i + 1;
		B = &A + i;
	}
}

byte printf(word string, byte args){
	byte i = 0;
	byte j = 0;
	word char = &string + i;
	while(*char){
		char = &string + i;
		if(*char != '%'){
			if(*char != '\')
				print_char(*char);
			else{
				char = &string + i + 1;
				if(*char == 'n')
					print_char(0x0A);
				else if(*char == 't')
					print_char(0x09);
				else{
					print_char(*char);
				}
				i = i + 2;
				continue;
			}
		}else{
			char = &string + i + 1;
			byte arg = &args + j;
			if(*char == 'd')
				print_num(*arg, SIGNED);
			else if(*char == 'u')
				print_num(*arg, UNSIGNED);
			else if(*char == 'x')
				print_hex(*arg, 8);
			else if(*char == 'a')
				print_char(*arg);
			i = i + 2;
			j = j + 1;
			continue;
		}
		i = i + 1;
	}
}

printf("TABELA DE FORMATOS NUMERICOS: \n\n");
byte x = -1;
printf("\tSigned: 	%d \n   ",	x);
printf("\tUnsigned: %u \n",		x);
printf("\tHexa: 	%x \n",		x);
printf("\tAscii:   '%a'\n",		x);