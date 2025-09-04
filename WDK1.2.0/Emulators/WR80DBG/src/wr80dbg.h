#ifndef __WR80DBG_H__
#define __WR80DBG_H__
/*
	STANDARD LIBC INCLUDES CONDITIONS
	You can include those libraries before the wr80asm.h library
	'cause this ifndef conditions, however, it's not necessary include
	the wr80list.h and wr80data.h outside, except for new assembler versions.
*/
// -----------------------------------------------------------------------------
#ifndef _INC_STDIO
#include <stdio.h>
#endif
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif
#ifndef _INC_STRING
#include <string.h>
#endif
#ifndef _STDBOOL_H
#include <stdbool.h>
#endif
#ifndef _INC_CTYPE
#include <ctype.h>
#endif
#ifndef _MATH_H_
#include <math.h>
#endif
#include <winsock2.h>

#include "wr80data.h"	// WR80 Variables, Structs and Data for Assembler
// -----------------------------------------------------------------------------

void print_version(){
	const char* description = FILE_DESCRIPTION;
	const char* author = COMPANY_NAME;
	const char* version = VER_STRING;
	printf("\n********************************************************************************\n");
	printf("%s v%s\n", description, version);
	printf("Created by %s\n\n", author);
	printf("********************************************************************************\n");	
}

void print_help(){
	printf("Usage:\n");
    printf (" -d | --debug <hex_file|bin_file> : Debug the binary file\n" \
       		" -b | --binary : Debug the file in binary format\n" \
			" -lm | --listen-mode : When executed by emulator\n");
}

int CreateClient(int serverport){
	// Inicializa Winsock
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
        	closesocket(sock);
        	WSACleanup();
        	return 0;	
		}
    }
    
    print_version();
    printf("Client Connected in WR80EMU Server %s:%d! \n" \
			"Type s, r, c, d [addr] | exit\n", LOCALHOST, serverport);
    
    return 1;
}

void CloseClient(){
	closesocket(sock);
    WSACleanup();
}

void RunEmulator(char* binary, bool bin){
	char command[512];
	if(bin)
    	snprintf(command, sizeof(command), "Start wr80emu -ed %s -b", binary);
    else
    	snprintf(command, sizeof(command), "Start wr80emu -ed %s", binary);
    system(command);
}

void DebugCPUInfo(){
    while (1) {
        printf("> ");
        memset(message, 0, BUFFER_SIZE);
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("Finishing Client...\n");
            break;
        }
        
        if (strcmp(message, "c") == 0) {
            system("cls");
            continue;
        }
        
        send(sock, message, strlen(message), 0);
        memset(response, 0, BUFFER_SIZE);
        int bytes = recv(sock, response, BUFFER_SIZE, 0);
        if (bytes > 0) {
            response[bytes] = '\0';
            printf("WR80EMU Info: %s\n", response);
        }
    }
}
#endif
