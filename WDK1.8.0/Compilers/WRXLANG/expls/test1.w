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

func("Hello", "World", 'B', 'C', 'D');