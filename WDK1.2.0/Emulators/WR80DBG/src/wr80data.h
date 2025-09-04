#ifndef __WR80DATA_H__
#define __WR80DATA_H__

	#define SERVER_PORT 8080
	#define BUFFER_SIZE 1024
	#define LOCALHOST "127.0.0.1"
	
	int CreateClient(int);
	void CloseClient(void);
	void RunEmulator(char*, bool);
	void DebugCPUInfo(void);
	void print_version(void);
	void print_help(void);
	
	WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    
	
	
#endif
