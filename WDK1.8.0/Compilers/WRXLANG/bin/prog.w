byte print_hex(byte x){
	byte p;
	byte s = 8;
	
	print_ascii('0');
	print_ascii('x');
	
	while(s){
		s = s - 4;
		p = x >> s & 0x0F;
		if(p < 10) print_ascii(p + '0'); else print_ascii(p - 10 + 'A');
	}
}

byte print_ascii(byte x)	
	0x1003 = x;

byte x = 0x13;
print_hex(x);