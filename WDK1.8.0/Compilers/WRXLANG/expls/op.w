byte A = 10;
word B = 0x0100;
byte C = 2;

if((A == 10) && (B != 0) && (C == 1)){
    asm {"ED"}
    1;
}else{
    asm {"ED"}
    2;
}