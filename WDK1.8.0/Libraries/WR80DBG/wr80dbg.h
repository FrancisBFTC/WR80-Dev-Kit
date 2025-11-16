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
#ifndef _STDINT_H
	#include <stdint.h>
#endif

#include "../COMMON/compat.h"
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

void print_commands(){
	printf(	"Debug commands: \n" \
			" h | help  : Print these commands\n" \
			" c | clear : Clear the debugging window\n" \
			" x | exit  : Exit the debugging window\n" \
			" r | regs  : See the currently registers \n" \
			" s | step  : Run the code step-by-step (Step Into)\n" \
			" e | exec  : Execute the code until next breakpoint\n" \
			" bp <addr> : Insert a breakpoint into hexa <addr>\n" \
			" rb <addr> : Remove a breakpoint from hexa <addr>\n" \
			" d  <addr> : Dump the RAM code bytes from hexa <addr>\n" \
			"    e.g. d 0FA, d PC, etc.\n" \
			" ds <addr> : Dump the STACK data bytes from hexa <addr>\n" \
			"    e.g. ds FF0, ds SP, etc.\n");
}

void DebugCPUInfo(){
	bool exec_mode = false;
    while (1) {
        printf("> ");
        memset(message, 0, BUFFER_SIZE);
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

		if (strcmp(message, "x") == 0 || strcmp(message, "exit") == 0) {
	        printf("Finishing Debugger...\n");
	        break;
	    }
	        
	    if (strcmp(message, "c") == 0 || strcmp(message, "clear") == 0) {
	       	system(CLEAR);
	        continue;
	    }
	    
	    if(strcmp(message, "h") == 0 || strcmp(message, "help") == 0) {
			print_commands();
			continue;
		}
	    
        if(!exec_mode){
	        send(sock, message, strlen(message), 0);
	        memset(response, 0, BUFFER_SIZE);
	        int bytes = recv(sock, response, BUFFER_SIZE, 0);
	        if (bytes > 0) {
	            response[bytes] = '\0';
	            printf("WR80EMU Info: ");
	            print_colored_response(response);
	            printf("\n");
	            
	            if (strcmp(message, "e") == 0 || strcmp(message, "exec") == 0) {
	            	exec_mode = true;
	            	// Deixar socket nao-bloqueante
	            	BlockingSocket(false);
	        	}
	        }
		}else{
			bool allowed_cmd = strncmp(message, "bp ", 3) == 0 || strncmp(message, "rb ", 3) == 0 || 
								strcmp(message, "e") == 0;
			int bytes;
			memset(response, 0, BUFFER_SIZE);
			
			if (allowed_cmd) {
	            send(sock, message, strlen(message), 0);
	        	int bytes = recv(sock, response, BUFFER_SIZE, 0);
	        	if(bytes > 0){
		        	response[bytes] = '\0';
		            printf("WR80EMU Info: ");
		            print_colored_response(response);
		            printf("\n");
				}
				continue;
	        }
	        
	        // Deixar socket nao-bloqueante
			BlockingSocket(false);
			memset(response, 0, BUFFER_SIZE);
	        bytes = recv(sock, response, BUFFER_SIZE, 0);
	        if(bytes > 0){
	        	response[bytes] = '\0';
	        	if(strncmp(response, "\n PC:", 5) == 0){
	        		exec_mode = false;
	        		printf("WR80EMU Info: ");
					const char* str = "Breakpoint found."; 
					printf(YELLOW "Breakpoint found." RESET);
		            print_colored_response(response);
		            printf("\n");
				}
			}else{
				printf("WARNING: Emulator in execution mode.\n");	
			}
			BlockingSocket(true);
	        
		}
    }
}

// Testa se é um número hexadecimal (ex: 00, FFF, 0x00D)
int is_hex_number(const char *s) {
    if (strncmp(s, "0x", 2) == 0 || strncmp(s, "0X", 2) == 0) {
        s += 2;
    }
    while (*s) {
        if (!isxdigit((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

// Testa se é um mnemônico (ex: JP, MOV, ADD ... só letras maiúsculas)
int is_mnemonic(const char *s) {
    int len = strlen(s);
    if (len == 0) return 0;
    for (int i = 0; i < len; i++) {
        if (!isupper((unsigned char)s[i]))
            return 0;
    }
    return 1;
}

// Decide a cor do token
void print_token(const char *token, int hasColon) {
    if (is_hex_number(token) || token[0] == '-') {
        printf(PINK "%s" RESET, token);
    }else if (hasColon) {
    	// registrador (antes de ':')
        printf(YELLOW "%s" RESET, token);
    } else if (is_mnemonic(token)) {
        printf(BLUE "%s" RESET, token);
    } else if(token[0] == '(') {
    	printf(GREEN "%s" RESET, token);
	}else {
        printf(YELLOW "%s" RESET, token);
    }
}


void print_colored_response(char *response) {
    char token[128];
    int i = 0;
	
	response[strlen(response)] = '\0';
	
    while (*response) {
        // quebra tokens por espaço, vírgula ou dois pontos
        if (isspace((unsigned char)*response) || *response == ',' || *response == ':') {
            if (i > 0) {
                token[i] = '\0';

                // Decide a cor
                int hasColon = (*response == ':');  // verifica se terminou em ':'
        		print_token(token, hasColon);
                i = 0;
            }
            putchar(*response); // imprime o separador
            response++;
        } else {
            if (i < (int)sizeof(token)-1)
                token[i++] = *response;
            response++;
        }
    }

    // Último token (se existir)
    if (i > 0) {
        token[i] = '\0';
        print_token(token, 0);
    }
}


#endif
