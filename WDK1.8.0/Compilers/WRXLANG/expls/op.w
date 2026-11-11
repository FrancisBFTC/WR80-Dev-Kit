byte W = 65;
if(1){
word A = &W;
byte X = *A;

if(X == 65){
	asm{"ED"}
	1;
}else{
	asm{"ED"}
	2;
}
}