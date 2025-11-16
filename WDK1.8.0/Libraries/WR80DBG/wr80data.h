#ifndef __WR80DATA_H__
#define __WR80DATA_H__

	#define SERVER_PORT 8080
	#define BUFFER_SIZE 1024
	#define LOCALHOST "127.0.0.1"
	
	// Códigos de cor ANSI (versões brilhantes)
	#define RESET   "\033[0m"
	#define YELLOW 	"\033[38;2;255;255;0m"   	// amarelo brilhante (registradores)
	#define PINK 	"\033[38;2;255;105;180m"   	// rosa brilhante (números hexadecimais)
	#define BLUE 	"\033[96m"   				// azul brilhante (mnemônicos)
	#define GREEN	"\033[92m"	 				// verde brilhante (numeros entre parenteses)

	void DebugCPUInfo(void);
	void print_version(void);
	void print_help(void);
	void print_commands(void);
	void print_colored_response(char*);
	
    struct sockaddr_in server;
    
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    	
#endif
