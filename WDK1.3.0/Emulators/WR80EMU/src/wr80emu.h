/*
	WR80 Emulator Functions Library
	Created by Wender Francis (KiddieOS.Community)
	Date: 30/08/2025

*/

#ifndef __WR80EMU_H__
#define __WR80EMU_H__

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

#include <errno.h>
#include <stdint.h>

#if _WIN32	// Windows

	#include <conio.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

#else 		// Linux

	#include "linux/linuxc.h"
	
#endif 		// _WIN32 End

#include "wr80emu_data.h"

/**
 * Funï¿½ï¿½o: load_hex
 * Lï¿½ um arquivo texto com bytes hexadecimais (2 dï¿½gitos por byte separados por espaï¿½o).
 * Primeira linha deve ser "v2.0 raw".
 * Aloca dinamicamente a memï¿½ria e retorna via parï¿½metro.
 * Retorna a quantidade de bytes carregados ou -1 em caso de erro.
 */
int load_hex(const char *filename, unsigned char **memory) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo HEX");
        return -1;
    }

    char line[1024];

    // Lï¿½ a primeira linha e verifica o header
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }
    line[strcspn(line, "\r\n")] = 0; // remover \n

    if (strcmp(line, "v2.0 raw") != 0) {
        fprintf(stderr, "Erro: Header invalido (esperado 'v2.0 raw')\n");
        fclose(fp);
        return -1;
    }

    // Primeiro passo: contar quantos bytes existem
    size_t count = 0;
    unsigned int byte_val;
    while (fscanf(fp, "%x", &byte_val) == 1) {
        count++;
    }

    if (count == 0) {
        fprintf(stderr, "Erro: Nenhum dado HEX encontrado\n");
        fclose(fp);
        return -1;
    }

    // Aloca a memï¿½ria
    *memory = malloc(memory_size);
    clear_ram(*memory);
    if (!*memory) {
        fprintf(stderr, "Erro: Falha ao alocar memoria (%zu bytes)\n", count);
        fclose(fp);
        return -1;
    }

    // Volta o ponteiro do arquivo para depois do header
    rewind(fp);
    fgets(line, sizeof(line), fp); // descarta novamente a primeira linha

    // Lï¿½ de novo, agora armazenando
    size_t pos = 0;
    while (fscanf(fp, "%x", &byte_val) == 1) {
        (*memory)[pos++] = (unsigned char)byte_val;
    }

    fclose(fp);
    return (int)pos; // quantidade de bytes lidos
}

/**
 * Funï¿½ï¿½o: load_bin
 * Lï¿½ um arquivo binï¿½rio, aloca dinamicamente a memï¿½ria
 * e retorna via parï¿½metro.
 * Retorna a quantidade de bytes carregados ou -1 em caso de erro.
 */
int load_bin(const char *filename, unsigned char **memory) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Erro ao abrir arquivo BIN");
        return -1;
    }

    // Descobre o tamanho do arquivo
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    if (filesize <= 0) {
        fprintf(stderr, "Erro: Arquivo BIN vazio ou invalido\n");
        fclose(fp);
        return -1;
    }

    // Aloca memï¿½ria
    *memory = malloc(memory_size);
    clear_ram(*memory);
    if (!*memory) {
        fprintf(stderr, "Erro: Falha ao alocar memoria (%ld bytes)\n", filesize);
        fclose(fp);
        return -1;
    }

    // Lï¿½ o arquivo inteiro
    size_t read_bytes = fread(*memory, 1, filesize, fp);

    fclose(fp);

    if (read_bytes != (size_t)filesize) {
        fprintf(stderr, "Aviso: Apenas %zu/%ld bytes lidos\n", read_bytes, filesize);
    }

    return (int)read_bytes;
}

// hex_dump: Print hexadecimal bytes from machine code in a formatted way
// -----------------------------------------------------------------------------
void hex_dump(unsigned char* code, uint16_t address, int size){
	printf("\nCode Length: %d\n", size);
	for(int i = 0; i < size; i++){
		if(i % 16 == 0)
			printf("\n0x%03X:", address);

		printf(" %02X", code[address]);
		address++;
	}
}

char* hexdump_dbg(unsigned char* code, uint16_t address, int size){
	static char resp[BUFFER_SIZE];
	uint16_t addr_start = address;
	size = (0x1000 - addr_start > 80) ? 80 : (0x1000 - addr_start);
	snprintf(resp, sizeof(resp), "\nSize: %d\n", size);
	
	for(int i = 0; i < size; i++){
		if(address > 0xFFF)
			break;
		if(i % 16 == 0){
			if(i != 0){
				address -= 16;
				snprintf(resp, sizeof(resp), "%s |", resp);
				for(int j = 0; j < 16; j++){
					if(code[address + j] <= 0x0D)
						snprintf(resp, sizeof(resp), "%s.", resp);
					else
						snprintf(resp, sizeof(resp), "%s%c", resp, code[address + j]);
						
				}
				snprintf(resp, sizeof(resp), "%s|", resp);
				address += 16;
			}
			
			snprintf(resp, sizeof(resp), "%s\n0x%03X:", resp, address);
			addr_start = address;	
		}

		snprintf(resp, sizeof(resp), "%s %02X", resp, code[address]);
		address++;
	}
	
	if(addr_start < 0x1000){
		uint16_t size_addr = (address - addr_start); 
		address -= size_addr;
		int lenspaces = (16 - size_addr) * 3;
		for(int i = 0; i < lenspaces; i++)
			snprintf(resp, sizeof(resp), "%s ", resp);
			
		snprintf(resp, sizeof(resp), "%s |", resp);
		for(int j = 0; j < size_addr; j++){
			if(code[address + j] <= 0x0D)
				snprintf(resp, sizeof(resp), "%s.", resp);
			else
				snprintf(resp, sizeof(resp), "%s%c", resp, code[address + j]);				
		}
		snprintf(resp, sizeof(resp), "%s|", resp);
	}
				
	return resp;
}
// -----------------------------------------------------------------------------

int16_t sign_extend(uint16_t value) {
    // Mantï¿½m apenas os 12 bits vï¿½lidos
    value &= 0x0FFF;
    if (value & 0x800) {
        value |= 0xF000;
    }
    return (int16_t)value;
}

void print_bin4(uint8_t value) {
    value &= 0x0F; // garante que sï¿½ ficam 4 bits

    for (int i = 3; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("b\n");
}

void clear_ram(unsigned char* code){
	if (code != NULL) {
        memset(code, 0, MAX_MEMORY);
    }
}

void activate_debug(bool on){
	debug_mode = on;
}

char* get_cpu_info(){
    static char response[BUFFER_SIZE];
	
	snprintf(response, sizeof(response), "\n PC: %03X, SP: %03X, BP: %03X, DR: %02X,  SR: ", PC, SP, BP, DR);
	uint8_t value = SR & 0x0F;
    for (int i = 3; i >= 0; i--) {
        snprintf(response, sizeof(response), "%s%d", response, (value >> i) & 1);
    }
    snprintf(response, sizeof(response), "%sb\n", response);
	
	for(int i = 0; i < 8; i++)
		snprintf(response, sizeof(response), "%s R%d: %02X, ", response, i, RX[i]);
	snprintf(response, sizeof(response), "%s\n", response);
	for(int i = 0; i < 8; i++)
		snprintf(response, sizeof(response), "%s P%d: %02X, ", response, i, PX[i]);
	snprintf(response, sizeof(response), "%s\n", response);
	
	snprintf(response, sizeof(response), "%s0x%03X: ", response, PC);
	
	if(!isExtension){
		switch(ram[PC] & 0xF0){
			case 0xD0:
			case 0xE0:
			case 0xF0: {
				int16_t offs = ((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
				offs = sign_extend((uint16_t)offs);
				snprintf(response, sizeof(response), "%s%02X%02X \t%s %d (0x%03X)", response, curr_opcode, next_opcode, mnemonics[mnemonic], offs, PC + offs + 2);
				break;
			}
		
			case 0x60:	snprintf(response, sizeof(response), "%s%02X \t%s 0x%02X", response, curr_opcode, mnemonics[mnemonic], ram[PC] & 0x0F);
						break;
			
			case 0xA0:	snprintf(response, sizeof(response), "%s%02X \t%s %d", response, curr_opcode, mnemonics[mnemonic], ram[PC] & 0x07);
						break;
					
			case 0x80:
			case 0x90:	snprintf(response, sizeof(response), "%s%02X \t%s %s", response, curr_opcode, mnemonics[mnemonic], port_registers[ram[PC] & 0x07]);
					   	break;
					   	
			default:	snprintf(response, sizeof(response), "%s%02X \t%s %s", response, curr_opcode, mnemonics[mnemonic], user_registers[ram[PC] & 0x07]);
						break;
		}	
	}else{
		switch(ram[PC] & 0xF0){
			case 0x00:
			case 0x10:
			case 0x20:	snprintf(response, sizeof(response), "%s%02X \t%s", response, curr_opcode, mnemonics[mnemonic]);
					 	break;
			case 0x30:
			case 0x40:	snprintf(response, sizeof(response), "%s%02X \t%s %s", response, curr_opcode, mnemonics[mnemonic], user_registers[ram[PC] & 0x07]);
					 	break;
			case 0x50:
			case 0x70: {
				int16_t offs = ((curr_opcode & 0x07) << 8) | ((curr_opcode & 0x20) << 6) | (next_opcode & 0xFF);
				offs = sign_extend((uint16_t)offs);
				snprintf(response, sizeof(response), "%s%02X%02X \t%s %d (0x%03X)", response, curr_opcode, next_opcode, mnemonics[mnemonic], offs, PC + offs + 2);
				break;
			}
		}
		
	}

    return response;
}

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

int GetConnection(bool dbg){
    int c;
    
    // Escutar
    listen(server_fd, 3);
    //printf("Servidor aguardando conexoes na porta %d...\n", PORTA);
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
    
    //char* response = get_cpu_info();
	//send(client_fd, response, strlen(response), 0);
	
    return 1;
}

void CloseServer(){
	closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
}

uint16_t get_address(char* addr){
	if (strcmp(addr, "PC") == 0)
		return PC;
	else if(strcmp(addr, "SP") == 0)
		return SP;
	else if(strcmp(addr, "BP") == 0)
		return BP;
	else if(strcmp(addr, "DR") == 0)
		return (uint16_t)DR & 0xFF;
	else if(strncmp(addr, "R", 1) == 0){
		char numstr[2];
		char* endptr;
		sscanf(addr + 1, "%s", numstr);
		uint8_t num = (uint8_t) strtol(numstr, &endptr, 10);
		return RX[num];
	}else if(strncmp(addr, "P", 1) == 0){
		char numstr[2];
		char* endptr;
		sscanf(addr + 1, "%s", numstr);
		uint8_t num = (uint8_t) strtol(numstr, &endptr, 10);
		return PX[num];
	}
}

int execute_command(const char* request){
	if(strcmp(request, "s") == 0 || strcmp(request, "step") == 0){
		char* response = get_cpu_info();
		send(client_fd, response, strlen(response), 0);
		return 1;
	}else if(strcmp(request, "r") == 0 || strcmp(request, "regs") == 0){
		char* response = get_cpu_info();
		send(client_fd, response, strlen(response), 0);
		return 0;
	}else if(strcmp(request, "e") == 0 || strcmp(request, "exec") == 0){
		activate_debug(false);
		proc_dd();
		exec_mode = true;
		const char* response = "Program executed.";
		send(client_fd, response, strlen(response), 0);
		return 1;	
	}else if (strncmp(request, "d ", 2) == 0) {
		char addrstr[5];
		char* endptr;
		sscanf(request + 2, "%s", addrstr);
		uint16_t addr = strtol(addrstr, &endptr, 16);
		if(*endptr != '\0'){
			addr = get_address(addrstr);
		}
		char* response;
		if(addr < 0x1000){
			response = hexdump_dbg(ram, addr, 16 * 5);
		}else{
			response = "Error => Invalid memory.";
		}
		
		send(client_fd, response, strlen(response), 0);
		return 0;
	}else if (strncmp(request, "ds ", 3) == 0) {
		char addrstr[5];
		char* endptr;
		sscanf(request + 3, "%s", addrstr);
		uint16_t addr = strtol(addrstr, &endptr, 16);
		if(*endptr != '\0'){
			addr = get_address(addrstr);
		}
		char* response;
		if(addr < 0x1000){
			response = hexdump_dbg(stack, addr, 16 * 5);
		}else{
			response = "Error => Invalid memory.";
		}
		
		send(client_fd, response, strlen(response), 0);
		return 0;
	}else if (strncmp(request, "bp ", 3) == 0){
		char addrstr[5];
		char response[100];
		char* endptr;
		sscanf(request + 3, "%s", addrstr);
		uint16_t addr = strtol(addrstr, &endptr, 16);
		if(*endptr != '\0'){
			addr = get_address(addrstr);
		}
		if(!breaks){
			breaks = malloc(memory_size);
		}
		if(addr < 0x1000){
			breaks[addr] = 0x01;
			snprintf(response, sizeof(response), "Insert Breakpoint at address 0x%03X.", addr);
		}else{
			snprintf(response, sizeof(response), "Error => Invalid memory.");
		}
		
		send(client_fd, response, strlen(response), 0);
		return 0;
	}else if (strncmp(request, "rb ", 3) == 0){
		char addrstr[5];
		char response[100];
		char* endptr;
		sscanf(request + 3, "%s", addrstr);
		uint16_t addr = strtol(addrstr, &endptr, 16);
		if(*endptr != '\0'){
			addr = get_address(addrstr);
		}
		if(breaks){
			if(addr < 0x1000){
				if(breaks[addr]){
					breaks[addr] = 0x00;
					snprintf(response, sizeof(response), "Remove breakpoint at address 0x%03X.", addr);
				}else{
					snprintf(response, sizeof(response), "Warning => There is no breakpoint here.");
				}
			}else{
				snprintf(response, sizeof(response), "Error => Invalid memory.");
			}	
		}else{
			snprintf(response, sizeof(response), "Error => There is no available breakpoints.");
		}
		
		send(client_fd, response, strlen(response), 0);
		return 0;
	}else {
		const char* response = "Unknown Command.";
		send(client_fd, response, strlen(response), 0);
		return 0;
	}	
}

void debug_process(bool dbg){
	if(exec_mode){
		if(!breaks){
			breaks = malloc(memory_size);
		}
		if(breaks[PC]){
			proc_ed();
			exec_mode = false;
			// Deixar socket bloqueante
			u_long mode = 0;	// 1 = non-blocking, 0 = blocking
		    ioctlsocket(client_fd, FIONBIO, &mode);
		    
		    char* response = get_cpu_info();
		    send(client_fd, response, strlen(response), 0);
		}else{
			exec_mode = true;
			// Deixar socket não-bloqueante
			u_long mode = 1;	// 1 = non-blocking, 0 = blocking
		    ioctlsocket(client_fd, FIONBIO, &mode);
		
		    char buffer[1024];
		    int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
		    if(bytes > 0){
		    	if (strncmp(buffer, "bp ", 3) == 0){
					char addrstr[5];
					char response[100];
					char* endptr;
					sscanf(buffer + 3, "%s", addrstr);
					uint16_t addr = strtol(addrstr, &endptr, 16);
					if(*endptr != '\0'){
						addr = get_address(addrstr);
					}
					if(!breaks){
						breaks = malloc(memory_size);
					}
					if(addr < 0x1000){
						breaks[addr] = 0x01;
						snprintf(response, sizeof(response), "Insert Breakpoint at address 0x%03X.", addr);
					}else{
						snprintf(response, sizeof(response), "Error => Invalid memory.");
					}

					send(client_fd, response, strlen(response), 0);
				}
			}
			return;
		}
	}

	char request[BUFFER_SIZE];
	int bytes;
	
	if(!connected){
		if(CreateServer(SERVER_PORT)){
			if(GetConnection(dbg)){
				connected = true;
				exec_mode = false;	
			}
		}
	}
	
	if(!exec_mode){
		while (1) {
			memset(request, 0, BUFFER_SIZE);
			bytes = recv(client_fd, request, BUFFER_SIZE, 0);
			
			if (bytes <= 0) {
			    printf("Finishing Debugger.\n");
			    connected = false;
			    exec_mode = true;
			    activate_debug(false);
			    proc_dd();
			    CloseServer();
			    if(breaks)
			    	free(breaks);
			    break;
			}
			
			request[bytes] = '\0';
			
			if(execute_command(request))
				break;
		}
	}
}

void proc_and(){
	DR = DR & (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_or(){
	DR = DR | (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_not(){
	DR = ~(RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_xor(){
	DR = DR ^ (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_add(){
	uint16_t res = (uint16_t)DR + (uint16_t)(RX[curr_opcode & 0x0F]);
	DR = (uint8_t)res;
	SR = (res > 0xFF) ? SR | 0x1 : SR & 0xE;	// definir carry
	SR = (DR) ? SR & 0xD : SR | 0x2;			// definir zero
	clr = 0;
}

void proc_sub(){
	int16_t res = (int16_t)DR - (int16_t)(RX[curr_opcode & 0x0F]);
	DR = (uint8_t)res;
	SR = (res < 0) ? SR & 0xE : SR | 0x1;	// definir carry
	SR = (DR) ? SR & 0xD : SR | 0x2;		// definir zero
	clr = 0;
}

void proc_st(){
	DR = (DR & 0xF0) | (curr_opcode & 0x0F);
	clr = 0;
}

void proc_ld(){
	RX[curr_opcode & 0x0F] = DR;
	clr = 0;
}

void proc_in(){
	uint8_t ind = curr_opcode & 0x0F;
	if(ind > 1){
		if(ind == 2){
			uint16_t address = (uint16_t)((PX[0] & 0x0F) << 8) | (PX[1] & 0xFF);
			PX[ind] = ram[address];
		}else if(ind == 3){
			PX[ind] = _kbhit();
			if(PX[ind]){
				PX[ind] = _getch();
			}
		}		
	}
	DR = PX[ind];
	clr = 0;
}

void proc_out(){
	uint8_t ind = curr_opcode & 0x0F;
	PX[ind] = DR;
	if(ind > 1){
		if(ind == 2){
			uint16_t address = (uint16_t)((PX[0] & 0x0F) << 8) | (PX[1] & 0xFF);
			ram[address] = PX[ind];
		}else if(ind == 3){
			putchar(PX[ind]);
		}		
	}
	clr = 0;
}

void proc_shr(){
	DR = DR >> (curr_opcode & 0x07);
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_shl(){
	int16_t res = (int16_t)DR << (curr_opcode & 0x07);
	DR = (uint8_t)res;
	SR = (res > 0xFF) ? SR | 0x1 : SR & 0xE;
	SR = (DR) ? SR & 0xD : SR | 0x2;
	clr = 0;
}

void proc_bt(){
	uint8_t DR_temp = DR;
	proc_sub();
	DR = DR_temp;
	clr = 0;
}

void proc_jc(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += (SR & 0x1) ? OFFSET : 0;
	clr = 0;
}

void proc_jz(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += (SR & 0x2) ? OFFSET : 0;
	clr = 0;
}

void proc_jp(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += OFFSET;
	clr = 0;
}

void proc_ei(){
	SR = SR | 0x08;
	if(!di_state){
		ISR = (uint16_t)((PX[0] & 0x0F) << 8) | (PX[1] & 0xFF);	
	}
	di_state = false;
	clr = 0;
}

void proc_di(){
	SR = SR & 0x07;
	di_state = true;
	clr = 0;	
}

void proc_ed(){
	SR = SR | 0x04;
	activate_debug(true);
	exec_mode = false;
	clr = 0;
}

void proc_dd(){
	SR = SR & 0x0B;
	activate_debug(false);
	exec_mode = true;
	clr = 0;
}

void proc_ec(){
	SR = SR | 0x01;
	clr = 0;
}

void proc_dc(){
	SR = SR & 0x0E;
	clr = 0;
}

void proc_cdr(){
	DR = 0;
	clr = 0;
}

void proc_clr(){
	if(clr){
		system("cls");
		clr = 0;
		return;
	}
	for(int i = 0; i < 8; i++){
		RX[i] = 0;
		PX[i] = 0;
	}
	DR = 0;
	SR = 0;
	clr = 1;
}

void proc_pushb(){
	STLR = (uint8_t)(BP & 0xFF);
	STHR = (uint8_t)((BP & 0xF00) >> 8);
	stack[--SP] = (char)STHR;
	stack[--SP] = (char)STLR;
	clr = 0;
}

void proc_popb(){
	STLR = (uint8_t)(stack[SP++] & 0xFF);
	STHR = (uint8_t)(stack[SP++] & 0x0F);
	BP = (uint16_t)(((uint16_t)STHR & 0x0F) << 8) | STLR;
	clr = 0;
}

void proc_pushs(){
	STLR = (uint8_t)(SP & 0xFF);
	STHR = (uint8_t)((SP & 0xF00) >> 8);
	stack[--SP] = (char)STHR;
	stack[--SP] = (char)STLR;
	clr = 0;
}

void proc_pops(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0x0F;
	SP = (uint16_t)((STHR & 0x0F) << 8) | STLR;
	clr = 0;
}

void proc_sbp(){
	DR = (uint8_t)stack[BP - (uint16_t)DR];
	clr = 0;
}

void proc_abp(){
	DR = (uint8_t)stack[BP + (uint16_t)DR];
	clr = 0;
}

void proc_ssp(){
	SP = SP - DR;
	clr = 0;
}

void proc_iret(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0xFF;
	PC = (uint16_t)((STHR & 0x0F) << 8) | STLR;
	SR = (STHR & 0xF0) >> 4;
	PC -= 1;
	clr = 0;
}

void proc_pushd(){
	STLR = (uint8_t)(DR & 0xFF);
	stack[--SP] = STLR;
	clr = 0;
}

void proc_popd(){
	STLR = (stack[SP++] & 0xFF);
	DR = STLR;
	clr = 0;
}

void proc_sbw(){
	stack[BP - DR] = RX[0];
	clr = 0;
}

void proc_scr(){
	printf("%d Not implemented yet! PC = %03X\n", curr_opcode, PC);
}

void proc_scs(){
	printf("%d Not implemented yet! PC = %03X\n", curr_opcode, PC);
}

void proc_pusha(){
	printf("%d Not implemented yet! PC = %03X\n", curr_opcode, PC);
}

void proc_popa(){
	printf("%d Not implemented yet! PC = %03X\n", curr_opcode, PC);
}

void proc_ret(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0xFF;
	PC = (uint16_t)((STHR & 0x0F) << 8) | STLR;
	clr = 0;
}

void proc_push(){
	STLR = RX[curr_opcode & 0x07];
	stack[--SP] = (char)STLR;
	clr = 0;
}

void proc_pop(){
	STLR = (uint8_t)stack[SP++];
	RX[curr_opcode & 0x07] = STLR;
	clr = 0;
}

void proc_callpos(){
	proc_call(0x07);
}

void proc_callneg(){
	proc_call(0x0F);
}

void proc_call(uint8_t isol){
	OFFSET = (uint16_t)((curr_opcode & isol) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	STLR = (uint8_t)(PC & 0xFF);
	STHR = (uint8_t)((PC & 0xF00) >> 8);
	stack[--SP] = STHR;
	stack[--SP] = STLR;
	PC += OFFSET;
	clr = 0;
}

// emulate: Emulate the WR80 Code bytes
// -----------------------------------------------------------------------------
bool emulate_buffer(unsigned char* code, int size, bool dbg){
	int i;
	bool isFound;
	uint8_t opcode;
	ram = code;
	stack = malloc(memory_size);
    clear_ram(stack);
	while(PC < size){
		opcode = code[PC] & 0xF0;
		isExtension = (((uint8_t)code[PC] & EXTENSION) != 0) && (opcode != 0x60) && 
							(opcode != 0xD0) && (opcode != 0xE0) && (opcode != 0xF0);
		i = (isExtension) ? 16 : 0;
		isFound = false;
		for(; i < OPCODES_SIZE; i++){
			uint8_t opcode_arr = opcodes[i] & 0xF0;
			if(isExtension){
				if(opcode == 0x00 || opcode == 0x10 || opcode == 0x20){
					isFound = (code[PC] == opcodes[i]);
				}else if(opcode == 0x30 || opcode == 0x40 || opcode == 0x50 || opcode == 0x70){
					isFound = opcode == opcode_arr;
				}
			}else{
				isFound = opcode == opcode_arr;
			}
					
			if(isFound){
				curr_opcode = code[PC];
				if(PC + 1 < size) next_opcode = code[PC + 1];
				mnemonic = i;
				if(debug_mode || SR & 0x04 || connected)
					debug_process(dbg);
					
				cpu_operation = (void(*)())process[i];
				cpu_operation();
				break;
			}
		}
		if(!isFound){
			printf("Error: opcode not found on PC = %d! Code = %02X\n", PC, code[PC]);
			return isFound;
		}
		
		PC = PC + 1;
		PC &= 0xFFF;
	}
	
	if(connected){
		CloseServer();
	}
}
// -----------------------------------------------------------------------------

#endif
