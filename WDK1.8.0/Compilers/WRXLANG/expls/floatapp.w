/*
include "../../../Libraries/SYS8/WRX/float.w"

word A = 0x4248; // PI
word B = 0x4170; // e
word C = 0x3555; // 1/3
print_float_16(fadd(fsub(A, B), C)); //~0.76

//word A = 0x3C00;  // 1.0
//word B = 0x7C00;  // Inf
//print_float_16(fsub(A, B));
*/

/*
include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 0x3C00;
word B = 0x3800;
word C = fadd(A, B);

print("A = %f", C);
asm{"ED"}
*/

