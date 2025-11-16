#ifndef WINC_H
#define WINC_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <winsock2.h>	
	#include <windows.h>
	#include <process.h>
	#include <ws2tcpip.h>
	#include <errno.h>
	
#define THREAD	HANDLE
#define CLEAR 	"cls"
//#define Sleep(ms) 		usleep((ms) * 1000)

WSADATA wsa;
SOCKET sock;
void CloseClient();

#ifdef __WR80EMU_H__
	#include "../../WR80EMU/wr80data.h"
#endif
#ifdef __WR80DBG_H__
	#include "../../WR80DBG/wr80data.h"
#endif

#ifdef __WR80EMU_H__
int CreateServer(int serverport){
    // Inicializa Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Winsock Initialization error. Code: %d\n", WSAGetLastError());
        return 0;
    }

    // Cria socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error in create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serverport);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind Error. Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 0;
    }
	
    return 1;
}

void CloseServer(){
	closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
}

int GetConnection(bool dbg){
    int c;
    
    listen(server_fd, 3);

    if(!dbg)
		system("start wr80dbg --listen-mode");
	
    c = sizeof(struct sockaddr_in);
    client_fd = accept(server_fd, (struct sockaddr*)&client, &c);
    if (client_fd == INVALID_SOCKET) {
        printf("Accept Error. Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 0;
    }
	
    return 1;
}
#endif

#ifdef __WR80DBG_H__
int CreateClient(int serverport){
	
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WinSock Initialization fail. Code: %d\n", WSAGetLastError());
        return 0;
    }

    // Cria socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error in create socket: %d\n", WSAGetLastError());
        WSACleanup();
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
    closesocket(sock);
    WSACleanup();
}
#endif


void BlockingSocket(bool isBlock){
	// Deixar socket bloqueante
	// 1 = non-blocking, 0 = blocking
	u_long mode = !isBlock;
	#ifdef __WR80DBG_H__
		ioctlsocket(sock, FIONBIO, &mode);
	#else
		ioctlsocket(client_fd, FIONBIO, &mode);
	#endif
}

#ifdef __WR80DBG_H__
void enableVTMode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // habilita processamento de sequências ANSI
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

void RunEmulator(char* binary, bool bin){
	char command[512];
	if(bin)
		snprintf(command, sizeof(command), "Start wr80emu -ed %s -b", binary);
	else
	    snprintf(command, sizeof(command), "Start wr80emu -ed %s", binary);
	system(command);
}
	
#endif
