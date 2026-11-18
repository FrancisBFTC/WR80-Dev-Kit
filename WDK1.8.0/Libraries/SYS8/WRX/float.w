byte UNSIGNED 	= 0;
byte SIGNED		= 1;

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
	
	if((x & 0x7FFF) == 0x7C00){
		print_char('I');
		print_char('n');
		print_char('f');
		return;
	}
	
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

word fadd(word A, word B){
    byte signA;
    byte signB;
    byte exponentA;
    byte exponentB;
    byte exponent;
    byte sign;
    byte diff;

    word mantissaA;
    word mantissaB;
    word mantissa;
    word result;
    word temp;

    // Decodifica A
    signA = (A >> 15) & 1;
    exponentA = (A >> 10) & 0x1F;
    mantissaA = A & 0x03FF;

    // Decodifica B
    signB = (B >> 15) & 1;
    exponentB = (B >> 10) & 0x1F;
    mantissaB = B & 0x03FF;

    // Zero: retorna o outro operando
    if((A & 0x7FFF) == 0)
        return B;

    if((B & 0x7FFF) == 0)
        return A;

    // Recupera o bit implícito dos normalizados
    if(exponentA != 0)
        mantissaA = mantissaA + 1024;
    else
        exponentA = 1;

    if(exponentB != 0)
        mantissaB = mantissaB + 1024;
    else
        exponentB = 1;

    // Alinha os expoentes
    if(exponentA > exponentB){
        diff = exponentA - exponentB;
		mantissaB = mantissaB >> diff;
        exponent = exponentA;
    }else{
        diff = exponentB - exponentA;
		mantissaA = mantissaA >> diff;
        exponent = exponentB;
    }

    // Soma ou subtrai as mantissas
    if(signA == signB){
        sign = signA;
        mantissa = mantissaA + mantissaB;

        // Ajusta overflow da mantissa
        if(mantissa >= 2048){
            mantissa = mantissa >> 1;
            exponent = exponent + 1;
        }
    }else{
        // Escolhe o maior módulo
        if(mantissaA >= mantissaB){
            sign = signA;
            mantissa = mantissaA - mantissaB;
        }else{
            sign = signB;
            mantissa = mantissaB - mantissaA;
        }

        // Resultado zero
        if(mantissa == 0)
            return 0;

        // Normaliza à esquerda
        while((mantissa < 1024) && (exponent > 1)){
            mantissa = mantissa << 1;
            exponent = exponent - 1;
        }
    }

    // Overflow do expoente: infinito
    if(exponent >= 31){
        result = 0x7C00;

        if(sign)
            result = result | 0x8000;

        return result;
    }

    // Converte resultado subnormal
    if((exponent == 1) && (mantissa < 1024))
        exponent = 0;

    // Monta o WORD final
    result = sign;
    result = result << 15;

    temp = exponent;
    temp = temp << 10;

    result = result | temp;
    result = result | (mantissa & 0x03FF);

    return result;
}


// Subtração: A - B = A + (-B)
word fsub(word A, word B){
    if(B & 0x8000)
        B = B & 0x7FFF;
    else
        B = B | 0x8000;

    return fadd(A, B);
}
