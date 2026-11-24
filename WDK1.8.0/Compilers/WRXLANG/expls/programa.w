include "..\..\..\Libraries\SYS8\WRX\iosys.w"

byte numeros[3] = {1, 2, 3};
byte i = 1;

numeros[1] = 5;

if(numeros[i] == 5) {
    print("Encontrado!");
}

print("numero: %d\n", numeros[i]);

asm{"ED"}