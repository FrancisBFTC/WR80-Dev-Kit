word A = 0x980;
word B = 0x1070;

byte C = !(A > B);
if(C){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}