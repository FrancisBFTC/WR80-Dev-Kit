byte A = 0xFF;
byte B = 0xFF;

word C = !(A != B);
if(C){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}