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

/*
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

    if (!block) { // Sem hibernaÃ§Ã£o
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

static int _kbhit(void) {
    int ch = __read_key(0);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


static int _getch(void) {
    return __read_key(1);
}
*/

/*
static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard() {
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO); // Desabilita o modo canônico e o echo
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void reset_keyboard() {
    tcsetattr(0, TCSANOW, &initial_settings); // Restaura as configurações iniciais do terminal
}

int _kbhit() {
    char ch;
    int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if (nread == 1) {
        peek_character = ch;
        return 1;
    }
    
    return 0;
}


int _getch() {
    char ch;

    if (peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0, &ch, 1);
    return ch;
}
*/

// Source - https://stackoverflow.com/a
// Posted by PBS
// Retrieved 2025-11-16, License - CC BY-SA 3.0

void enable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

bool _kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

int _getch() {
	return getchar();
}


/* Limpa a tela */
static void clrscr(void) {
    printf("\033[H\033[J");
}

#include <sys/wait.h>

void RunInTerminal(const char* cmd) {
    const char* terms[] = {
        "xterm -e",
        "gnome-terminal --",
        "konsole -e",
        "xfce4-terminal -e"
    };

    char full[512];
    char check[128];

    for (int i = 0; i < 4; i++) {
        // Extrai o nome do terminal (primeira palavra)
        char prog[64];
        sscanf(terms[i], "%63s", prog);

        // Verifica se existe no PATH
        snprintf(check, sizeof(check), "command -v %s >/dev/null 2>&1", prog);
        if (system(check) != 0) continue;

        // Constrói comando usando sh -c
        snprintf(full, sizeof(full),
                 "%s \"sh -c '%s'\" &",
                 terms[i], cmd);

        system(full);
        return;
    }

    // Última tentativa: rodar sem terminal
    snprintf(full, sizeof(full), "%s &", cmd);
    system(full);
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
    fcntl(client_fd, F_SETFL, ((isBlock) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK)); 
}

#endif /* LINUXC_H */
