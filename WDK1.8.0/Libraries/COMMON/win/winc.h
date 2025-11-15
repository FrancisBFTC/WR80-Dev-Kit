#ifndef WINC_H
#define WINC_H

	#include <conio.h>
	#include <winsock2.h>	
	#include <windows.h>
	#include <process.h>
	#include <ws2tcpip.h>
	#include <errno.h>
	
#define THREAD	HANDLE
#define CLEAR 	"cls"

WSADATA wsa;

#include "../../WR80EMU/wr80emu_data.h"


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

void BlockingSocket(bool isBlock){
	// Deixar socket bloqueante
	// 1 = non-blocking, 0 = blocking
	u_long mode = !isBlock;
	ioctlsocket(client_fd, FIONBIO, &mode);
}
	
#endif
