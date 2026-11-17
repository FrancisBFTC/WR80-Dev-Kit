/*
include "../../../Libraries/SYS8/WRX/iosys.w"
include "../../../Libraries/SYS8/WRX/math.w"

word A = 1;
byte B = 8;
word C = A << B;
print("Val: %wu\n", C);
print("Factorial: %wu", factorial_16(8));
asm{"ED"}
*/

/*
include "../../../Libraries/SYS8/WRX/iosys.w"

word A = 255;
word B = 255;
word str = "Hello World!";
word intro = "Testando Parsing de WORDs!";

print("\n %s \n*** %s ***\n\n", &str, &intro);
print(" Resultado : %wu\n", A * B);
*/

/*
include "../../../Libraries/SYS8/WRX/iosys.w"
word A = 0x3C00;
word B = 0x4200;
word C = 0x3800;
word D = 0x3555;
word E = 0x4000;
word F = 0xBC00;

print("A = %f\n", A);
print("B = %f\n", B);
print("C = %f\n", C);
print("D = %f\n", D);
print("E = %f\n", E);
print("F = %f\n", F);
asm{"ED"}
*/

/*
include "../../../Libraries/SYS8/WRX/iosys.w"
byte test(){
    byte k = 12;
    word fraction = 2048;

    fraction = fraction * 16; //fraction << (16-k);

    print("K=%u FRAC=%wu\n", k, fraction);
}

test();
asm{"ED"}
*/
