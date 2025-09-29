#ifndef __WR80DATA_H__
#define __WR80DATA_H__
	
	// Estrutura para armazenar os arquivos
	typedef struct {
	    char name[10];	// nome do arquivo (nome 5.3)
	    uint16_t addr;	// endereco do arquivo
	    uint16_t size;	// tamanho do arquivo em bytes (máx 4096)
	} FileEntry;

	void print_version(void);
	void print_usage(void);
	void create_image(const char*, int);
	void list_files(const char*, FileEntry**, size_t*, size_t*);
	void save_to_binary(const char*, const char*, const char*, FileEntry*, size_t);
	void create_image(const char*, int);
	char *load_file(const char*);
	unsigned char* load_hex(const char*, size_t*);
	void writeHex(const char*, const char*);
	void writeBin(const char*, const char*);
	void write_binary(const char *, const char*, int);
	long get_size_file(const char*);
	char* changeExtension(const char*, const char*);

#endif
