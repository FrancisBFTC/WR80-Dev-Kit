byte X = "O fatorial de 5 e:";
byte Y = 5;
word B = 0;
byte i = 0;

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
		if(x >= y || !x)
			0x1003 = x / y + '0';
		x = x % y;
		y = y / 10;
	}
}

byte print_info(){
	while(*B != 0){
		B = &X + i;
		0x1003 = *B;
		i = i + 1;
	}
}

word main(){
	print_info();
	print_num(fact(Y));
}

main();

