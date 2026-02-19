byte Y = 5;

byte fact(byte a) {
	if(a == 0 || a == 1){
		1;
	}else{
		a * fact(a - 1);
	}
}

word main(){
	fact(Y);
}

main();

