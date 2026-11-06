word A = 0x9090;
word B = 0x80A0;

word C = A < B;
if(C){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}