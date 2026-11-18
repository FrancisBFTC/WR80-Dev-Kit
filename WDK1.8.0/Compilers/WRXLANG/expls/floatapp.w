include "../../../Libraries/SYS8/WRX/float.w"

word A = 0x3C00; // 1.0
word B = 0x3800; // 0.5

print_float_16(A);
print_char('+');
print_float_16(B);
print_char('=');
print_float_16(fadd(A, B));