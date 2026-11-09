word A = 0x0101;
word P = &A;
byte B = 1;

if((*P > 256) && (B == 1)){
    asm {"ED"}
    1;
}else{
    asm {"ED"}
    2;
}
