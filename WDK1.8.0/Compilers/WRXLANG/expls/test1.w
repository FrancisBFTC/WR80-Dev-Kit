/*
byte func(word X, word Z, byte Y){
	word i = 0;
	while(*(X + i)){
		0x1003 = *(X + i);
		i = i + 1;
	}
	i = 0;
	word B = *(&X + 2);
	while(*(B + i)){
		0x1003 = *(B + i);
		i = i + 1;
	}
	byte j = 1;
	0x1003 = Y;
	0x1003 = *(&Y + j);
	0x1003 = *(&Y + j + 1);
}
*/

/*
byte func(word X, word Z, byte Y){
	word i = 0;
	while(*(X + i)){
		0x1003 = *(X + i);
		i = i + 1;
	}
	i = 0;
	while(*(Z + i)){
		0x1003 = *(Z + i);
		i = i + 1;
	}
	byte j = 1;
	byte W = &Y;
	byte A = *(W + j);
	byte B = *(W + j + 1);
	0x1003 = Y;
	0x1003 = A;
	0x1003 = B;
}
*/
/*
byte func(word X, word Z, byte Y){
	//word i = 0;
	while(*X){
		0x1003 = *X;
		X = X + 1;
		//i = i + 1;
	}
	//i = 0;
	//byte C = *Z;
	while(*Z){
		0x1003 = *Z;
		Z = Z + 1;
		//i = i + 1;
	}
	//byte j = 1;
	//byte W = &Y;
	//byte W = &X + 4;
	//byte A = *((&X + 4) + 1);
	//byte B = *((&X + 4) + 2);
	0x1003 = Y;
	0x1003 = *((&X + 4) + 1);
	0x1003 = *((&X + 4) + 2);
}
*/

byte func(word X, word Z){
	while(*(X++))	0x1003 = *X;
	while(*(Z++))	0x1003 = *Z;
}

func("Hello", "World");