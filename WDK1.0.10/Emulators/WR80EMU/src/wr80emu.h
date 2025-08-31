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
#include <conio.h>
#include <windows.h>
#include <fcntl.h>   // <-- define _O_TEXT, _O_BINARY etc
#include <io.h>      // <-- define _open_osfhandle e companhia

#include "wr80emu_data.h"

/**
 * Função: load_hex
 * Lê um arquivo texto com bytes hexadecimais (2 dígitos por byte separados por espaço).
 * Primeira linha deve ser "v2.0 raw".
 * Aloca dinamicamente a memória e retorna via parâmetro.
 * Retorna a quantidade de bytes carregados ou -1 em caso de erro.
 */
int load_hex(const char *filename, unsigned char **memory) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo HEX");
        return -1;
    }

    char line[1024];

    // Lê a primeira linha e verifica o header
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

    // Aloca a memória
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

    // Lê de novo, agora armazenando
    size_t pos = 0;
    while (fscanf(fp, "%x", &byte_val) == 1) {
        (*memory)[pos++] = (unsigned char)byte_val;
    }

    fclose(fp);
    return (int)pos; // quantidade de bytes lidos
}

/**
 * Função: load_bin
 * Lê um arquivo binário, aloca dinamicamente a memória
 * e retorna via parâmetro.
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

    // Aloca memória
    *memory = malloc(memory_size);
    clear_ram(*memory);
    if (!*memory) {
        fprintf(stderr, "Erro: Falha ao alocar memoria (%ld bytes)\n", filesize);
        fclose(fp);
        return -1;
    }

    // Lê o arquivo inteiro
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
// -----------------------------------------------------------------------------

int16_t sign_extend(uint16_t value) {
    // Mantém apenas os 12 bits válidos
    value &= 0x0FFF;
    if (value & 0x800) {
        value |= 0xF000;
    }
    return (int16_t)value;
}

void print_bin4(uint8_t value) {
    value &= 0x0F; // garante que só ficam 4 bits

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

void debug_process(int index){
	printf("\nPC: %03X, SP: %03X, BP: %03X, DR: %02X, SR: ", PC, SP, BP, DR);
	print_bin4(SR);
	
	for(int i = 0; i < 8; i++)
		printf("R%d: %02X, ", i, RX[i]);
	printf("\n");
	for(int i = 0; i < 8; i++)
		printf("P%d: %02X, ", i, PX[i]);
	printf("\n");
	printf("0x%03X: ", PC);
	
	if(!isExtension){
		switch(ram[PC] & 0xF0){
			case 0xD0:
			case 0xE0:
			case 0xF0: {
				int16_t offs = ((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
				offs = (int16_t)sign_extend((uint16_t)offs);
				printf("%s %d (0x%03X)", mnemonics[index], offs, PC + offs + 2);
				break;
			}
		
			case 0x60:	printf("%s 0x%02X", mnemonics[index], ram[PC] & 0x0F);
						break;
			
			case 0xA0:	printf("%s %d", mnemonics[index], ram[PC] & 0x07);
						break;
					
			case 0x80:
			case 0x90:	printf("%s %s", mnemonics[index], port_registers[ram[PC] & 0x07]);
					   	break;
					   	
			default:	printf("%s %s", mnemonics[index], user_registers[ram[PC] & 0x07]);
						break;
		}	
	}else{
		switch(ram[PC] & 0xF0){
			case 0x00:
			case 0x10:
			case 0x20:	printf("%s", mnemonics[index]);
					 	break;
			case 0x30:
			case 0x40:	printf("%s %s", mnemonics[index], user_registers[ram[PC] & 0x07]);
					 	break;
			case 0x50:
			case 0x70: {
				int16_t offs = ((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
				offs = (int16_t)sign_extend((uint16_t)offs);
				printf("%s %d (0x%03X)", mnemonics[index], offs, PC + offs + 2);
				break;
			}
		}
		
	}
	char command[50];
	bool step_state = true;
	char addrstr[5];
	char* endptr;
	char ch;
	printf("\n");
	while(step_state){
		printf("$ ");
		ch = getchar();
		if(ch == 'd'){
			int i = 0;
			while((ch = getchar()) != '\n'){
				if(ch != ' ') addrstr[i++] = ch;
				addrstr[i] = 0; 
			}
			uint16_t addr = strtol(addrstr, &endptr, 16);
			hex_dump(ram, addr, 16 * 5);
			printf("\n");
			continue;
		}
		while(getchar() != '\n');
		
		if(ch == 's'){
			step_state = false;
		}else if(ch == 'r'){
			step_state = false;
			activate_debug(step_state);
			proc_dd();
		}else if(ch == 'c'){
			system("cls");
		}
	}
}

void proc_and(){
	DR = DR & (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;  
}

void proc_or(){
	DR = DR | (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
}

void proc_not(){
	DR = ~(RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
}

void proc_xor(){
	DR = DR ^ (RX[curr_opcode & 0x0F]);
	SR = (DR) ? SR & 0xD : SR | 0x2;
}

void proc_add(){
	uint16_t res = (uint16_t)DR + (uint16_t)(RX[curr_opcode & 0x0F]);
	DR = (uint8_t)res;
	SR = (res > 0xFF) ? SR | 0x1 : SR & 0xE;	// definir carry
	SR = (DR) ? SR & 0xD : SR | 0x2;			// definir zero
}

void proc_sub(){
	int16_t res = (int16_t)DR - (int16_t)(RX[curr_opcode & 0x0F]);
	DR = (uint8_t)res;
	SR = (res < 0) ? SR & 0xE : SR | 0x1;	// definir carry
	SR = (DR) ? SR & 0xD : SR | 0x2;		// definir zero
}

void proc_st(){
	DR = (DR & 0xF0) | (curr_opcode & 0x0F);
}

void proc_ld(){
	RX[curr_opcode & 0x0F] = DR;
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
}

void proc_shr(){
	DR = DR >> (curr_opcode & 0x07);
	SR = (DR) ? SR & 0xD : SR | 0x2;
}

void proc_shl(){
	int16_t res = (int16_t)DR << (curr_opcode & 0x07);
	DR = (uint8_t)res;
	SR = (res > 0xFF) ? SR | 0x1 : SR & 0xE;
	SR = (DR) ? SR & 0xD : SR | 0x2;
}

void proc_bt(){
	uint8_t DR_temp = DR;
	proc_sub();
	DR = DR_temp;
}

void proc_jc(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += (SR & 0x1) ? OFFSET : 0;
}

void proc_jz(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += (SR & 0x2) ? OFFSET : 0;
}

void proc_jp(){
	OFFSET = (uint16_t)((curr_opcode & 0x0F) << 8) | (next_opcode & 0xFF);
	OFFSET = sign_extend(OFFSET);
	PC += 1;
	PC += OFFSET;
}

void proc_ei(){
	SR = SR | 0x08;
	if(!di_state){
		ISR = (uint16_t)((PX[0] & 0x0F) << 8) | (PX[1] & 0xFF);	
	}
	di_state = false;
}

void proc_di(){
	SR = SR & 0x07;
	di_state = true;	
}

void proc_ed(){
	SR = SR | 0x04;
}

void proc_dd(){
	SR = SR & 0x0B;
}

void proc_ec(){
	SR = SR | 0x01;
}

void proc_dc(){
	SR = SR & 0x0E;
}

void proc_cdr(){
	DR = 0;
}

void proc_clr(){
	for(int i = 0; i < 8; i++){
		RX[i] = 0;
		PX[i] = 0;
	}
	DR = 0;
	SR = 0;
}

void proc_pushb(){
	STLR = (uint8_t)(BP & 0xFF);
	STHR = (uint8_t)((BP & 0xF00) >> 8);
	stack[--SP] = (char)STHR;
	stack[--SP] = (char)STLR;
}

void proc_popb(){
	STLR = (uint8_t)(stack[SP++] & 0xFF);
	STHR = (uint8_t)(stack[SP++] & 0x0F);
	BP = (uint16_t)(((uint16_t)STHR & 0x0F) << 8) | STLR;
}

void proc_pushs(){
	STLR = (uint8_t)(SP & 0xFF);
	STHR = (uint8_t)((SP & 0xF00) >> 8);
	stack[--SP] = (char)STHR;
	stack[--SP] = (char)STLR;
}

void proc_pops(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0x0F;
	SP = (uint16_t)((STHR & 0x0F) << 8) | STLR;
}

void proc_sbp(){
	DR = (uint8_t)stack[BP - (uint16_t)DR];
}

void proc_abp(){
	DR = (uint8_t)stack[BP + (uint16_t)DR];
}

void proc_ssp(){
	SP = SP - DR;
}

void proc_iret(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0xFF;
	PC = (uint16_t)((STHR & 0x0F) << 8) | STLR;
	SR = (STHR & 0xF0) >> 4;
	PC -= 1;
}

void proc_pushd(){
	STLR = (uint8_t)(DR & 0xFF);
	stack[--SP] = STLR;
}

void proc_popd(){
	STLR = (stack[SP++] & 0xFF);
	DR = STLR;
}

void proc_sbw(){
	stack[BP - DR] = RX[0];
}

void proc_scr(){
	printf("%d Not implemented yet!\n", curr_opcode);
}

void proc_scs(){
	printf("%d Not implemented yet!\n", curr_opcode);
}

void proc_pusha(){
	printf("%d Not implemented yet!\n", curr_opcode);
}

void proc_popa(){
	printf("%d Not implemented yet!\n", curr_opcode);
}

void proc_ret(){
	STLR = (stack[SP++] & 0xFF);
	STHR = stack[SP++] & 0xFF;
	PC = (uint16_t)((STHR & 0x0F) << 8) | STLR;
}

void proc_push(){
	STLR = RX[curr_opcode & 0x07];
	stack[--SP] = (char)STLR;
}

void proc_pop(){
	STLR = (uint8_t)stack[SP++];
	RX[curr_opcode & 0x07] = STLR;
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
	STHR = (uint8_t)(PC & 0xF00) >> 8;
	stack[--SP] = STHR;
	stack[--SP] = STLR;
	PC += OFFSET;
}

// emulate: Emulate the WR80 Code bytes
// -----------------------------------------------------------------------------
bool emulate_buffer(unsigned char* code, int size){
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
				if(debug_mode || SR & 0x04)
					debug_process(i);
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
}
// -----------------------------------------------------------------------------

#endif
