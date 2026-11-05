/*
word A = 0x0102 * 0x0304;
word B = 0x00FF * 0x00FF;
word C = 0x0100 * 0x0100;
word D = 0x0101 * 0x0101;
word E = 0x1234 * 0x0002;
word F = 0xFFFF * 0x0002;
asm{"ED"}
*/

/*
word X = 0;
word Y = 0;

X = 0x0102; Y = 0x0304;
word A = X * Y;

X = 0x00FF; Y = 0x00FF;
word B = X * Y;

X = 0x0100; Y = 0x0100;
word C = X * Y;

X = 0x0101; Y = 0x0101;
word D = X * Y;

X = 0x1234; Y = 0x0002;
word E = X * Y;

X = 0xFFFF; Y = 0x0002;
word F = X * Y;

X = 0; Y = 0;

asm{"ED"}
*/

/*
// Teste 1 (funcional)
word A = 0x0102;
word B = 0x0304;
word C = A * B + 1;
asm{"ED"}
*/

/*
// Teste 2 (funcional)
word A = 0x0102;
word B = 0x0304;
word C = A * B - 1;
asm{"ED"}
*/

/*
// Teste 3 (funcional)
word A = 0x0102;
word B = 0x0304;
word D = 0x0002;
word E = 0x0003;

word C = A * B + D * E;

asm{"ED"}
*/

/*
// Teste 4 (funcional)
word A = 0x0102;
word B = 0x0304;
word C = A * B + 1 + 2;

asm{"ED"}
*/

/*
word A = 0;
word B = 0;
word D = 0;
word E = 0;
word F = 0;
word G = 0;

word T1 = 0;
word T2 = 0;
word T3 = 0;
word T4 = 0;
word T5 = 0;
word T6 = 0;
word T7 = 0;
word T8 = 0;
word T9 = 0;

// Valores atribuídos em tempo de execução.
A = 0x0102;
B = 0x0304;
D = 0x0002;
E = 0x0003;
F = 0x0004;
G = 0x0005;

// TESTE 1: três multiplicações na mesma expressão.
T1 = A * B + D * E + F * G;

// TESTE 2: multiplicação com soma no operando direito.
T2 = A * (B + D);

// TESTE 3: multiplicação com operações nos dois operandos.
T3 = (A + B) * (D + E);

// TESTE 4: multiplicação com subtração aninhada.
T4 = A * (B - D);

// TESTE 5: multiplicação de um resultado de outra multiplicação.
T5 = (A * B) * (D + E);

// TESTE 6: soma aninhada à direita.
T6 = A * B + (D * E + F * G);

// TESTE 7: subtração com uma expressão aninhada.
T7 = A * B - (D * E + F * G);

// TESTE 8: multiplicação de uma soma que contém outra multiplicação.
T8 = (A * B + D * E) * F;

// TESTE 9: multiplicações encadeadas com associatividade à esquerda.
T9 = A * B + D * E * F;
asm{"ED"}
*/
/*
word A = 1000;
word B = 10;
word C = 2;

word X = A / B;
word Y = A % B;

word Z = C + (A / B);
word W = C * (A / B);

word K = A / (B / C);
*/

/*
word A = 1000;
word B = 10;
word C = 2;
word D = 3;
word E = 37;
word F = 13;

word T1 = A / (B / C);
word T2 = A % (B / D);
word T3 = (A / 7) % F;
word T4 = (A % E) / D;
word T5 = (A / B) / 5;
word T6 = A / (B % 6);
word T7 = ((A % 99) % 7);
word T8 = (A / 99) * ((A % 99) + 1);
*/
/*
word T9  = ((12345 / 37) * 3) + ((9876 % 89) * 2);
word T10 = ((5000 / (17 + 3)) + (4095 % 64)) * 2;
word T11 = (65535 / 256) + (65535 % 256);
word T12 = (30000 % 257) + (30000 / 257);
*/

/*
word A1 = 65535;
word B1 = 257;

word T13 = A1 / B1;
word T14 = A1 % B1;

word A2 = 65535;
word B2 = 256;

word T15 = A2 / B2;
word T16 = A2 % B2;
*/

/*
word T17 = 257 / 258;
word T18 = 65535 / 3;
word T19 = 65535 % 3;
word T20 = 257 % 258;
*/
/*
word A = 1000;
word B = 10;
word C = A / B;
word D = A % B;
*/

/*
word X = 0xE002;
word Y = 0xD002;

if(X == Y){
	asm {"ED"}
	asm {"STD 0xFF"}
}else{
	asm {"ED"}
	asm{"STD 0x00"}
}
*/

/*
word A = 100;
word B = 20;
word C = 5;
word D = 0xF0;


if(A + B == 121){
	asm {"ED"}
	1;
}else{
	asm {"ED"}
	4;
}

if(A * 2 > B * 10){
	asm {"ED"}
	2;
}else{
	asm {"ED"}
	5;
}

if((A + B) / C != 23){
	asm {"ED"}
	3;
}else{
	asm {"ED"}
	6;
} 

if(D){
	asm {"ED"}
	7;
}else{
	asm {"ED"}
	8;
}
*/

word A = 0x100;
word B = 0x100;

/*
if(!(A & B)){
    asm {"ED"}
    1;
}
asm {"ED"}
2;
*/

/*
if(!(A | B)){
    asm {"ED"}
    1;
}
asm {"ED"}
2;
*/

/*
if(!(A ^ B)){
    asm {"ED"}
    1;
}
asm {"ED"}
2;
*/

/*
if(A & 0xFF00){
    asm {"ED"}
    1;
}
asm {"ED"}
2;
*/

/*
if(A | 0x0001){
    asm {"ED"}
    1;
}
asm {"ED"}
2;
*/

byte X = (A == B);
// +, -, *, / e %
if(X){
    asm {"ED"}
    1;
}
asm {"ED"}
2;