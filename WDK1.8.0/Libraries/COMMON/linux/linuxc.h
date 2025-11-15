#ifndef LINUXC_H
#define LINUXC_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>

#define INVALID_SOCKET 	-1
#define SOCKET_ERROR   	-1
#define Sleep(ms) 		usleep((ms) * 1000)
#define closesocket(s) 	close(s)
#define SOCKET 			int
#define THREAD			pthread_t
#define CLEAR 			"clear"

#include "../../WR80EMU/wr80emu_data.h"


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


static int _getch(void) {
    return __read_key(1);
}

static int _kbhit(void) {
    int ch = __read_key(0);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


/*
struct termios oldt, newt;

void init_keyboard(){
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);   // modo raw
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void reset_keyboard(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit(void) {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
}

int getch(void) {
    return getchar();
}
*/

/* Limpa a tela */
static void clrscr(void) {
    printf("\033[H\033[J");
}

void RunInTerminal(const char* cmd) {
    const char* terminals[] = {
        "xterm -e",
        "gnome-terminal --",
        "konsole -e",
        "xfce4-terminal -e"
    };

    char fullcmd[512];

    for (int i = 0; i < 4; i++) {
        snprintf(fullcmd, sizeof(fullcmd), "%s %s &", terminals[i], cmd);
        if (system(fullcmd) == 0)
            return;
    }

    // �ltima tentativa: rodar em background mesmo sem terminal
    snprintf(fullcmd, sizeof(fullcmd), "%s &", cmd);
    system(fullcmd);
}


int CreateServer(int serverport){
    // Cria socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket Initialization Error. Code: %d\n", errno);
		return 0;
	}

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serverport);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		printf("Bind Error. Code: %d\n", errno);
		close(server_fd);
		return 0;
	}
    return 1;
}

void CloseServer(){
    if (client_fd > 0) close(client_fd);
    if (server_fd > 0) close(server_fd);
}

int GetConnection(bool dbg){
    socklen_t c;
    
    listen(server_fd, 3);

    if(!dbg)
		RunInTerminal("wr80dbg --listen-mode");
	
	c = sizeof(struct sockaddr_in);
    client_fd = accept(server_fd, (struct sockaddr*)&client, &c);
    if (client_fd < 0) {
        printf("Accept Error. Code: %d\n", errno);
        close(server_fd);
		return 0;
	}
	
    return 1;
}

void BlockingSocket(bool isBlock){
	int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, ((isBlock) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK); 
}

#endif /* LINUXC_H */
