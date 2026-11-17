byte print_char(byte c) 
	0x1003 = c;

byte print_num_16(word x, byte t){
	word y = 10000;
	byte st = 0;
	
	if(!x){
		print_char('0');
		return;
	}
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


byte print_float_16(word x){
    byte sign;
    byte exponent;
    byte digit;
    byte i;
    byte k;
    word mantissa;
    word integer;
    word fraction;
    word mask;

    sign = (x >> 15) & 1;
    exponent = (x >> 10) & 0x1F;
    mantissa = x & 0x03FF;

    if(sign)
        print_char('-');

    if(exponent == 0){
        integer = 0;
        fraction = mantissa >> 8;
    }else{
        mantissa = mantissa + 1024;

        if(exponent >= 25){
            integer = mantissa << (exponent - 25);
            fraction = 0;
        }else{
            k = 25 - exponent;
			
            if(k < 16){
                integer = mantissa >> k;

                mask = 1;
                mask = mask << k;
                mask = mask - 1;
				
                fraction = mantissa & mask;
                fraction = fraction << (16 - k);
				
            }else if(k == 16){
                integer = 0;
                fraction = mantissa;
            }else{
                integer = 0;
                fraction = mantissa >> (k - 16);
            }
        }
    }

    print_num_16(integer, UNSIGNED);
    print_char('.');

    i = 0;

	while(i < 4){
        digit = 0;

        if(fraction >= 58983) digit = 9;
        else if(fraction >= 52429) digit = 8;
        else if(fraction >= 45876) digit = 7;
        else if(fraction >= 39322) digit = 6;
        else if(fraction >= 32768) digit = 5;
        else if(fraction >= 26215) digit = 4;
        else if(fraction >= 19661) digit = 3;
        else if(fraction >= 13108) digit = 2;
        else if(fraction >= 6554) digit = 1;

        print_char(digit + '0');
        fraction = fraction * 10;

        i = i + 1;
    }
}
