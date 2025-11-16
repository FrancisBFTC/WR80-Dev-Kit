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

SOCKET sock;

#ifdef __WR80EMU_H__
	#include "../../WR80EMU/wr80data.h"
#endif
#ifdef __WR80DBG_H__
	#include "../../WR80DBG/wr80data.h"
#endif


static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard() {
    tcgetattr(0, &initial_settings);
}

void reset_keyboard() {
    tcsetattr(0, TCSANOW, &initial_settings); // Restaura as configurações iniciais do terminal
}

int _kbhit() {
    char ch;
    int nread;

    if (peek_character != -1)
        return 1;

    struct termios temp = new_settings;  // cópia segura
    temp.c_cc[VMIN] = 0;

    tcsetattr(0, TCSANOW, &temp);
    nread = read(0, &ch, 1);
    tcsetattr(0, TCSANOW, &new_settings); // restaura SEM estragar nada

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

#ifdef __WR80EMU_H__
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
#endif

#ifdef __WR80DBG_H__
int CreateClient(int serverport){
    
    // Inicializa socket POSIX
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
		printf("Error in create socket: %d\n", errno);
		return 0;
	}

    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(LOCALHOST);

    // Conecta ao servidor
    int count = 0;
    while (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    	if(count++ == 10){
    		printf("Error in connect.\n");
    		CloseClient();
        	return 0;	
		}
		Sleep(100);
    }
    
    print_version();
    printf("Debugger Connected in WR80EMU Server %s:%d! \n" \
			"Type 'h' or 'help' to see the commands!\n", LOCALHOST, serverport);
    
    return 1;
}

void CloseClient(){
    close(sock);
}
#endif

void BlockingSocket(bool isBlock){
	#ifdef __WR80EMU_H__
		int flags = fcntl(client_fd, F_GETFL, 0);
		fcntl(client_fd, F_SETFL, ((isBlock) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK));
	#else
		int flags = fcntl(sock, F_GETFL, 0);
		fcntl(sock, F_SETFL, ((isBlock) ? flags & ~O_NONBLOCK : flags | O_NONBLOCK));
	#endif
}

void RunEmulator(char* binary, bool bin){
	char command[512];
	if(bin)
    	snprintf(command, sizeof(command), "wr80emu -ed %s -b", binary);
	else
	    snprintf(command, sizeof(command), "wr80emu -ed %s", binary);
	RunInTerminal(command);	
}

#endif /* LINUXC_H */
