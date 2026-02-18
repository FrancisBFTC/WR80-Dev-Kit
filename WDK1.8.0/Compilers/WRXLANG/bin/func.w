word B = 0;
byte i;
byte A = "Hello World! Programming in WRX!";

byte printinfo(){
	while(*B != 0){
		B = &A + i;
		0x1003 = *B;
		i = i + 1;
	}
}

printinfo();

