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
			" -b  | --boot <boot_file>: Boot file to insert in seek 0 (use --format)");
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
    
    printf("%d bytes of '%s' file added in seek %d\n", sizefile, srcBin, seek);
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
	
#endif
