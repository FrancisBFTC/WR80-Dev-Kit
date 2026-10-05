byte factorial(byte a) {
	if(a == 0 || a == 1){
		return 1;
	}else{
		return a * factorial(a - 1);
	}
}

byte termial(byte a){
	if(a == 0 || a == 1)
		return a;
	else
		return a + termial(a - 1);
}

byte isPrime(byte a){
	byte b = a;
	byte c = 0;
	if(a < 2)
		return 0;
	while(a){
		if(!(b % a))
			c = c + 1;
		a = a - 1;
	}
	if(c > 2)
		return 0;
	return 1;
}

byte primorial(byte a){
	if(a == 0 || a == 1)
		return 1;
	else{
		if(!isPrime(a))
			return primorial(a - 1);
		else
			return a * primorial(a - 1);
	}
}