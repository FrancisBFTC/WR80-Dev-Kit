word A = 0xFF;
word B = 0xFF;

word C = !(A / B);
if(C){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}