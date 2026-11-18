
/*
include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 1;
word C = A << 2;
print("Val: %wu\n", C);
*/

/*
include "../../../Libraries/SYS8/WRX/float.w"

word A = 0x3DA8; // sqrt(2)
word B = 0x4248; // pi
word C = 0x4170; // e
word D = 0x3555; // 1/3
word E = 0x2E66; // 0.1
word F = 0x399A; // 0.7
word G = 0x42AB; // 10/3
word I = 0x3E48; // pi/2

print_float_16(A);
print_char(' ');
print_float_16(B);
print_char(' ');
print_float_16(C);
print_char(' ');
print_float_16(D);
print_char(' ');
print_float_16(E);
print_char(' ');
print_float_16(F);
print_char(' ');
print_float_16(G);
print_char(' ');
print_float_16(I);
asm{"ED"}
*/

/*
include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 0x3DA8; // sqrt(2)
word B = 0x4248; // pi
word C = 0x4170; // e
word D = 0x3555; // 1/3

print("A = %f\n", A);
print("B = %f\n", B);
print("C = %f\n", C);
print("D = %f\n", D);
asm {"ED"}
*/

