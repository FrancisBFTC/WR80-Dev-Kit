byte fact(byte a) {
	if(a == 0 || a == 1){
		return 1;
	}else{
		return a * fact(a - 1);
	}
}