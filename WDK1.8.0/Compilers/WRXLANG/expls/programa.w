include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 1023;
word B = 1024;

if(A >= B){
	print("A e menor que B\n\n");
}else{
	print("A nao e menor que B\n\n");
}

asm {"ED"}
