byte A = 21;
byte B = 20;

word P = &A;
word Q = &B;

if(!(*P < *Q)){
    asm {"ED"}
    1;
}else{
    asm {"ED"}
    2;
}