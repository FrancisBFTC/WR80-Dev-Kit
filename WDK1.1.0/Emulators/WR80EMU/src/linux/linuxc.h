#ifndef LINUXC_H
#define LINUXC_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/* Lê uma tecla (hibernando ou não) */
static int __read_key(int block) {
    struct termios oldt, newt;
    int ch;
    int oldf = 0;

    // Salva estado atual
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Desativa o ICANON e o ECHO
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (!block) { // Sem hibernação
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    }

    ch = getchar();

    // Restaura estado
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (!block)
        fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}

/* Lê um caractere sem esperar Enter (hiberna até apertar algo) */
static int _getch(void) {
    return __read_key(1);
}

/* Verifica se alguma tecla foi pressionada (sem hibernação) */
static int _kbhit(void) {
    int ch = __read_key(0);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

/* Limpa a tela */
static void clrscr(void) {
    printf("\033[H\033[J");
}

#endif /* LINUXC_H */