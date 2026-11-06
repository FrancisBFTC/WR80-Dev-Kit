byte A = 0x80;
byte B = 0x70;

word C = !(A > B);
if(C){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}