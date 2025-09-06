#ifndef __WR80IMG_H__
#define __WR80IMG_H__

/*
	STANDARD LIBC INCLUDES CONDITIONS
	You can include those libraries before the wr80img.h library
	'cause this ifndef conditions, however, it's not necessary include
	the wr80list.h and wr80data.h outside, except for new wr80img versions.
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

#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "wr80data.h"
	
void print_version(){
	const char* description = FILE_DESCRIPTION;
	const char* author = COMPANY_NAME;
	const char* version = VER_STRING;
	printf("\n********************************************************************************\n");
	printf("%s v%s\n", description, version);
	printf("Created by %s\n\n", author);
	printf("********************************************************************************\n");	
}

void print_usage(){
	printf("Usage:\n");
    printf (" -s | --source <source_file> : Read the source file\n" \
       		" -o | --output <output_file> : Write the output file\n" \
			" -bs | --bytes <num> : Bytes to shift\n" \
			" -sk | --seek  <num> : File position to shift (multiply by bs)\n" \
			" -c  | --create <image_file> : Create an image file\n" \
			" -l  | --length <num> : Specify the bytes length of the image\n" \
			" -f  | --format : Format the image in WR80 FileSystem (WFS)\n" \
			" -b  | --boot <boot_file>: Boot file to insert in seek 0 (use --format)\n" \
			" -b2h : Convert bin to hex in -s and -o parameters (or -c)\n" \
			" -h2b : Convert hex to bin in -s and -o parameters\n");
}

// Função recursiva para listar arquivos
void list_files(const char *basePath, FileEntry **files, size_t *count, size_t *capacity) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[256];

    if ((dir = opendir(basePath)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);
        if (stat(path, &st) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recursão para subpastas
            list_files(path, files, count, capacity);
        } else if (S_ISREG(st.st_mode)) {
            // Arquivo normal
            if (*count >= *capacity) {
                *capacity *= 2;
                *files = realloc(*files, *capacity * sizeof(FileEntry));
                if (!*files) {
                    perror("realloc");
                    exit(1);
                }
            }

            FileEntry *entryPtr = &((*files)[*count]);
            strncpy(entryPtr->name, entry->d_name, sizeof(entryPtr->name) - 1);
            entryPtr->name[sizeof(entryPtr->name) - 1] = '\0';

            // Armazena tamanho (limitado a 12 bits)
            if (st.st_size > 0xFFF)
                entryPtr->size = 0xFFF; // limite
            else
                entryPtr->size = (uint16_t)st.st_size;

            (*count)++;
        }
    }
    
    closedir(dir);
}

// Função para salvar no arquivo binário
void save_to_binary(const char *binFile, const char* dir, const char* bootFile, FileEntry *files, size_t count) {
    uint16_t size_tmp = 0, addr_tmp = 0;
	FILE *f = fopen(binFile, "r+b");
    if (!f) {
        perror("fopen");
        return;
    }
	
	bool bootFound = false;
	for(size_t i = 0; i < count; i++){
		if(strcmp(files[i].name, bootFile) == 0){
			char path[100];
			files[i].addr = 0x000;
			snprintf(path, sizeof(path), "%s/%s", dir, bootFile);
			char *data = load_file(path);
			fwrite(data, files[i].size, 1, f);
			free(data);
			
			size_tmp = files[i].size;
    		addr_tmp = files[i].addr;
    		
			printf("%d bytes of '%s' file added in seek %d\n", size_tmp, path, addr_tmp);
			bootFound = true;
			break;
		}
	}
	if(!bootFound){
		printf("File '%s' not found!", bootFile);
		exit(EXIT_FAILURE);
	}
	
    bool nextFile = false;

    for (size_t i = 0; i < count; i++) {
    	bool isKernel = (strcmp(files[i].name, bootFile) != 0);
    	if(!nextFile)
			files[i].addr = (isKernel) ? addr_tmp + size_tmp + (14 * count) : 0x0000;
		else
			files[i].addr = (isKernel) ? addr_tmp + size_tmp : 0x0000;	
    	
        fwrite(&files[i], sizeof(FileEntry), 1, f);
        
		if(isKernel){
			nextFile = true;
			char path[100];
        	long pos = ftell(f);
        	fseek(f, files[i].addr, SEEK_SET);
        	snprintf(path, sizeof(path), "%s/%s", dir, files[i].name);
        	char *data = load_file(path);
        	fwrite(data, files[i].size, 1, f);
        	fseek(f, pos, SEEK_SET);
        	free(data);
        	size_tmp = files[i].size;
    		addr_tmp = files[i].addr;
    		printf("%d bytes of '%s/%s' file added in seek %d\n", size_tmp, dir, files[i].name, addr_tmp);
		}
    }

    fclose(f);
}

void create_image(const char *imageFile, int imageSize){
	FILE *f = fopen(imageFile, "wb");
    if (!f) {
        perror("Error: fopen error");
        return;
    }
    
    unsigned char zero = 0;
    fseek(f, imageSize - 1, SEEK_SET);
    fwrite(&zero, 1, 1, f);
    fclose(f);
}

char *load_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error in opening the file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize + 1);
    if (!buffer) {
        perror("Error in allocate memory");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    fclose(file);

    if (read_size != filesize) {
        fprintf(stderr, "Error: imcomplete reading of file\n");
        free(buffer);
        return NULL;
    }

    buffer[filesize] = '\0';

    return buffer;
}

unsigned char* load_hex(const char *filename, size_t *size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo HEX");
        return NULL;
    }

    char line[1024];

    // L? a primeira linha e verifica o header
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return NULL;
    }
    line[strcspn(line, "\r\n")] = 0; // remover \n

    if (strcmp(line, "v2.0 raw") != 0) {
        fprintf(stderr, "Erro: Header invalido (esperado 'v2.0 raw')\n");
        fclose(fp);
        return NULL;
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
        return NULL;
    }

	*size = count;
    unsigned char *memory = malloc(count);
    if (!memory) {
        fprintf(stderr, "Erro: Falha ao alocar memoria (%zu bytes)\n", count);
        fclose(fp);
        return NULL;
    }

    // Volta o ponteiro do arquivo para depois do header
    rewind(fp);
    fgets(line, sizeof(line), fp); // descarta novamente a primeira linha

    // L? de novo, agora armazenando
    size_t pos = 0;
    while (fscanf(fp, "%x", &byte_val) == 1) {
        memory[pos++] = (unsigned char)byte_val;
    }

    fclose(fp);
    return memory; // quantidade de bytes lidos
}

// Função para salvar no arquivo binário
void write_binary(const char *srcBin, const char* outBin, int seek) {
	FILE *f = fopen(outBin, "r+b");
    if (!f) {
        perror("fopen");
        return;
    }
    
    long sizefile = get_size_file(srcBin);
    char *data = load_file(srcBin);
    
    fseek(f, seek, SEEK_SET);
    fwrite(data, sizefile, 1, f);
    fclose(f);
    free(data);
    
    printf("%d bytes of '%s' file added in seek %d\n", sizefile, srcBin, seek);
}

// Função para salvar no arquivo binário
void writeBin(const char *srcHex, const char* outBin) {
    size_t filesize = 0;
    unsigned char *data = load_hex(srcHex, &filesize);
    if (!data) {
        fprintf(stderr, "Erro ao carregar HEX\n");
        return;
    }

    FILE *f = fopen(outBin, "wb");
    if (!f) {
        perror("Erro ao criar arquivo binário");
        free(data);
        return;
    }

    fwrite(data, 1, filesize, f);
    fclose(f);
    free(data);
    
    printf("binary file '%s' written successfully! (%zu bytes)\n", outBin, filesize);
}

void writeHex(const char *srcBin, const char* outHex){
	FILE *f = fopen(outHex, "w");
	if(!f){
		perror("Error in opening the file!\n");
		exit(1);
	}
	
	long sizefile = get_size_file(srcBin);
    char *data = load_file(srcBin);
    
	const char* header = "v2.0 raw";
	fprintf(f, "%s\n", header);
	
	for(size_t i = 0; i < sizefile; i++){
		fprintf(f, "%02X ", (uint8_t)data[i]);
		if((i + 1) % 16 == 0){
			fprintf(f, "\n");
		}
	}
	
	if(sizefile % 16 != 0){
		fprintf(f, "\n");
	}
	
	fclose(f);
	free(data);
	
	printf("Hexa file '%s' written successfully!\n", outHex);
}

long get_size_file(const char* filename){
	FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error in opening the file");
        return -1;
    }
    
	fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);
    fclose(file);
    
    return filesize;
}

char* changeExtension(const char *filename, const char* ext){
	char *newName = malloc(strlen(filename) + 1);
	strcpy(newName, filename);
	
	char *point = strrchr(newName, '.');
	if(point != NULL){
		strcpy(point, ext);
	}else{
		strcat(newName, ext);
	}
	
	return newName;
}
	
#endif
