byte UNSIGNED 	= 0;
byte SIGNED		= 1;

byte x = 254;

byte print_ascii(byte x)	0x1003 = x;

byte print_num(byte x, byte t){
	byte y = 100;
	if(x & 0x80 && t){
		x = -x;
		0x1003 = '-';
	}
	while(y != 0){
		if(x >= y || !x)
			0x1003 = x / y + '0';
		x = x % y;
		y = y / 10;
	}
}

print_num(x, UNSIGNED);
print_ascii(0x0A);
print_num(x, SIGNED);