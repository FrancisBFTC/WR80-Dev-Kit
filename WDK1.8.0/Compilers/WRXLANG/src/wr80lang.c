/*
	WR80 Compiler Executable
	Created by Wender Francis (KiddieOS.Community)
	Date: 16/02/2026

*/

#include "../wr80lang_private.h"
#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	//char source[] =	"byte A; A = 2;";
	
	if (argc == 1) {
		const char* description = FILE_DESCRIPTION;
		const char* author = COMPANY_NAME;
		const char* version = VER_STRING;
		printf("\n********************************************************************************\n");
		printf("%s v%s\n", description, version);
		printf("Created by %s\n\n", author);
		printf("********************************************************************************\n");
        printf("Usage:\n");
        printf (" -c | --compile <source_file> : Compile the source file\n\n");
		printf("Extra parameters:\n");
		printf (" -o | --output <output_file> : Generate the output file\n" \
				" -h | --hexdump : Show the hexa code after assembly (use -c before)\n" \
				" -b | --binary : Assemble the file in binary format\n" \
				" -a | --assembly [DATA | CODE | FUNC | FULL] : Show Specific Assembly Code\n");
        return EXIT_FAILURE;
    }
    
    bool comp = false;
	bool hexdump = false;
	bool output = false;
	bool bin = false;
	bool assembly = false;
	
	char* source = NULL;
	char* binary = NULL;
	char* asmtype = NULL;
	char* asm_code = NULL;
	char* bin_code = NULL;
	
	for(int i = 1; i < argc; i++){
		comp = (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compile") == 0) || comp;
		hexdump = (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hexdump") == 0) || hexdump;
		output = (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) || output;
		bin = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) || bin;
		assembly = (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--assembly") == 0) || assembly;
		if(source == NULL && comp)
			source = (argv[i + 1]) ? argv[i + 1] : NULL;
		if(binary == NULL && output)
			binary = (argv[i + 1]) ? argv[i + 1] : NULL;
		if(asmtype == NULL && assembly)
			asmtype = (argv[i + 1]) ? argv[i + 1] : NULL;
	}
	
	
	if(comp && source){
		long size_code;
		asm_code = load_file_to_buffer(source, &size_code);
    	bin_code = compile(asm_code);
    	
    	if(!bin_code) return EXIT_FAILURE;
    	
    	if(bin_code && assembly){
    		if(strcmp(asmtype, "DATA") == 0)		show_asm(_DATA);
    		else if(strcmp(asmtype, "CODE") == 0)	show_asm(_CODE);
    		else if(strcmp(asmtype, "FUNC") == 0)	show_asm(_FUNC);
    		else if(strcmp(asmtype, "FULL") == 0)	show_asm(_FULL);
			else{
				printf("Error: Invalid Assembly Type Parameter\n");
				return EXIT_FAILURE;
			}	
		}
		
    	if(bin_code && hexdump)	hex_dump(bin_code);
    	
		int length = get_code_size();
		int size_file;
		if(!bin){
			if(!output)
				binary = changeExtension(source, ".hex");
			size_file = writeHex(binary, bin_code, length);
		}else{
			if(!output)
				binary = changeExtension(source, ".bin");
			writeBin(binary, bin_code, length);
			size_file = length;
		}
		printf("\nThe file '%s' was compiled successfully with %d bytes!\n", binary, size_file);
		
	}else{
		printf("Error: Missing parameter -c/--compile or file source\n");
		return EXIT_FAILURE;
	}
    
	if(data_buf)	free(code_buf);
	if(code_buf)	free(data_buf);
	if(final_buf)	free(final_buf);
	if(asm_code)	free(asm_code);
	if(bin_code)	free(bin_code);
	if(binary)		free(binary);
	
	return EXIT_SUCCESS;
}
