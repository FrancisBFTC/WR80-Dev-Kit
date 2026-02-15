#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#ifdef _WIN32
	#ifdef BUILD_ASSEMBLER_DLL
		#define ASM_API __declspec(dllexport)
	#else
		#define ASM_API __declspec(dllimport)
	#endif
#endif

// Retorna ponteiro para o assembly gerado
ASM_API bool assemble_buffer(const char* source, unsigned char** code, bool verb);
ASM_API char *load_file_to_buffer(const char *filename, long *filesize);
ASM_API void hex_dump(unsigned char* code);
ASM_API int writeHex(const char *filename, unsigned char *machinecode, size_t size);
ASM_API void writeBin(const char *filename, unsigned char *machinecode, size_t size);
ASM_API char* changeExtension(const char *filename, const char* ext);
ASM_API int get_code_size();

#endif

