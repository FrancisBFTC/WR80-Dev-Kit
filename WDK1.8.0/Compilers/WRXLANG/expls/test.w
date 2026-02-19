byte x = 0x30;

byte func(byte x){
	byte y = x + 1;
	if(1){
		if(0){
			byte x = 0x32;
			0x1003 = x;
		}else{
			byte x = 0x34;
			0x1003 = x;
		}
	}
	0x1003 = y;
	return y + 2;
}

0x1003 = x;
x = func(x);
0x1003 = x;
