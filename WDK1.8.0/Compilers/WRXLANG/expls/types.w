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

word A = 1000;
word B = 10;

asm {"ED"}
word Q = A / B;
