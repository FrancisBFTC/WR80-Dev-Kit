main();

byte fact(byte a) {
	if(a == 0 || a == 1){
		1;
	}else{
		a * fact(a - 1);
	}
}

byte print_num(byte x){
	byte y = 100;
	while(y != 0){
		0x1003 = x / y + '0';
		x = x % y;
		y = y / 10;
	}
}

word main(){
	byte A = fact(5);
	print_num(A + A);
}

