byte W;
if(1){
	word A = 0x0100;
	word B = 0x0200;
	byte X = (A != 0) && (B != 0);

	if(X == 1){
		asm{"ED"}
		1;
	}else{
		asm{"ED"}
		2;
	}
}