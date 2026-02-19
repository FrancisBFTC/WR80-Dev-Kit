byte UNSIGNED 	= 0;
byte SIGNED		= 1;
byte str = "\tSigned: %d \n \tUnsigned: %u \n \tHexa: %x \n \tAscii: '%a'";
word B = 0;
byte i = 0;

byte print_ascii(byte x)	
	0x1003 = x;

byte print_num(byte x, byte t){
	byte y = 100;
	if(x & 0x80 && t){
		x = -x;
		print_ascii('-');
	}
	while(y != 0){
		if(x >= y || !x)
			print_ascii(x / y + '0');
		x = x % y;
		y = y / 10;
	}
}

byte print_hex(byte x, byte y){
	byte p;
	byte s = y;
	
	print_ascii('0');
	print_ascii('x');
	
	while(s){
		s = s - 4;
		p = x >> s & 0x0F;
		if(p < 10) print_ascii(p + '0'); else print_ascii(p - 10 + 'A');
	}
}

byte get_byte_info(byte x){
	while(*B){
		B = &str + i;
		if(*B != '%'){
			if(*B != '\')
				print_ascii(*B);
			else{
				B = &str + i + 1;
				if(*B == 'n')
					print_ascii(0x0A);
				else if(*B == 't')
					print_ascii(0x09);
				else{
					print_ascii(*B);
				}
				i = i + 2;
				continue;
			}
		}else{
			B = &str + i + 1;
			if(*B == 'd')
				print_num(x, SIGNED);
			else if(*B == 'u')
				print_num(x, UNSIGNED);
			else if(*B == 'x')
				print_hex(x, 8);
			else if(*B == 'a')
				print_ascii(x);
			i = i + 2;
			continue;
		}
		i = i + 1;
	}
}


byte x = 0x80;
get_byte_info(x);