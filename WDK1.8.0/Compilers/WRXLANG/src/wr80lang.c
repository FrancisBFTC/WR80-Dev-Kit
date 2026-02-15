#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	//char source[] =	"byte A; A = 2;";
	
	long size_code;
	char* source = load_file_to_buffer(argv[1], &size_code);
    char* bin = compile(source);
    
	if(bin) show_asm(_FULL);
    
    if(bin)	hex_dump((unsigned char*) bin);
    
    char* namebin = changeExtension(argv[1], ".hex");
    int size_file = writeHex(namebin, bin, (size_t)get_code_size());
    printf("\nThe file '%s' was compiled with %d bytes!\n", namebin, size_file);
    
	if(data_buf)	free(code_buf);
	if(code_buf)	free(data_buf);
	if(final_buf)	free(final_buf);	
	
	return 0;
}
