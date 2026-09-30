byte fact(byte a) {
	if(a == 0 || a == 1){
		return 1;
	}else{
		return a * fact(a - 1);
	}
}

byte term(byte a){
	if(a == 0 || a == 1)
		return a;
	else
		return a + term(a - 1);
}

byte isPrime(byte a){
	byte b = a;
	byte c = 0;
	if(a < 2)
		return 0;
	while(a){
		if(b % a == 0){
			c = c + 1;
		}
		a = a - 1;
	}
	if(c > 2)
		return 0;
	return 1;
}

byte prim(byte a){
	if(a == 0 || a == 1)
		return 1;
	else{
		if(!isPrime(a))
			return a - 1;
		else
			return a * prim(a - 1);
	}
}