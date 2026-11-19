include "../../../Libraries/SYS8/WRX/float.w"

word A = 0x4248; // PI
word B = 0x4170; // e
word C = 0x3555; // 1/3

print_float_16(fadd(fsub(A, B), C)); //~0.76