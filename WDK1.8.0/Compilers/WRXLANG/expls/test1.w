byte func(word X, byte Y){
	word i = 0;
	byte A = *X;
	while(A){
		0x1003 = A;
		i = i + 1;
		A = *(X + i);
	}
	byte j = 1;
	0x1003 = Y;
	0x1003 = *(&Y + j);
	0x1003 = *(&Y + j + 1);
}

func("Hello", 'B', 'C', 'D');