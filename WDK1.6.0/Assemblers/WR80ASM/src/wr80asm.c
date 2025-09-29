/*
	WR80 Assembler Executable
	Created by Wender Francis (KiddieOS.Community)
	Date: 20/08/2025

*/

#include "../wr80asm_private.h"
#include "wr80asm.h"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		const char* description = FILE_DESCRIPTION;
		const char* author = COMPANY_NAME;
		const char* version = VER_STRING;
		printf("\n********************************************************************************\n");
		printf("%s v%s\n", description, version);
		printf("Created by %s\n\n", author);
		printf("********************************************************************************\n");
        printf("Usage:\n");
        printf (" -m | --mount <source_file> : Assemble the source file\n" \
			 	" -e | --emulate <binary_file> : Emulate the binary file\n" \
				" -me | --mount-emulate <source_file> : Assemble and emulate the file\n\n");
		printf("Extra parameters:\n");
		printf (" -o | --output <output_file> : Generate the output file (use -m before)\n" \
				" -w | --write : Write the assembled file with emulating (use -me before)\n" \
				" -d | --debug : Debugging the code during emulating (use -m or -me before)\n" \
				" -h | --hexdump : Show the hexa code after assembly (use -m or -me before)\n" \
				" -h | --hexdump <binary_file> : Show the hexa code from binary file\n" \
				" -b | --binary : Assemble the file in binary format\n" \
				" -v | --verbose : Print assembler steps information\n" \
				" -a | --alloc : Allocate bytes when using ORG directive\n");
        return EXIT_FAILURE;
    }

	bool mount = false;
	bool hexdump = false;
	bool output = false;
	bool bin = false;
	bool verb = false;
	
	char* source = NULL;
	char* binary = NULL;
	
	for(int i = 1; i < argc; i++){
		mount = (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mount") == 0) || mount;
		hexdump = (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hexdump") == 0) || hexdump;
		output = (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) || output;
		bin = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) || bin;
		verb = (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) || verb;
		alloc = (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alloc") == 0) || alloc;
		if(source == NULL && mount)
			source = argv[i + 1];
		if(binary == NULL && output)
			binary = argv[i + 1];
	}
	
	/*
	char *source_code = load_file_to_buffer(source);
	if (!source_code) {
	    exit(EXIT_FAILURE);
	}
	
	bool mounted = (mount) ? assemble_buffer(source_code, &machinecode, verb) : false; 
	free(source_code);
	*/
	
	/*
	unsigned char* machine_code = NULL;
	char* assembly_code = 	"st 0x01 \r\n" \
							"shl 4 	 \r\n" \
							"st 0x02 \r\n" \
							"ld %r3";
	assemble_buffer(assembly_code, &machine_code, verb);
	hex_dump(machine_code);
	*/
	
	unsigned char* machinecode = NULL;
	bool mounted = (mount) ? assemble_file(source, &machinecode, verb) : false;
	//source = "getchar_ex.asm";
	//bool mounted = assemble_file(source, &machinecode, true);
	
	if(mounted && hexdump)
		hex_dump(machinecode);
		
	if(mounted){
		int length = get_code_size();
		int size_file;
		if(!bin){
			if(output){
				size_file = writeHex(binary, machinecode, length);
				printf("\nThe hexa file '%s' was assembled successfully with %d bytes!\n", binary, size_file);
			}else{
				binary = changeExtension(source, ".hex");
				size_file = writeHex(binary, machinecode, length);
				printf("\nThe hexa file '%s' was assembled successfully with %d bytes!\n", binary, size_file);
			}
		}else{
			if(output){
				writeBin(binary, machinecode, length);
				printf("\nThe binary file '%s' was assembled successfully with %d bytes!\n", binary, length);
			}else{
				binary = changeExtension(source, ".bin");
				writeBin(binary, machinecode, length);
				printf("\nThe binary file '%s' was assembled successfully with %d bytes!\n", binary, length);
			}
		}
	}
	
	if(machinecode != NULL)
		free(machinecode);
	if(binary != NULL)
		free(binary);
	close_lists();
	
    return EXIT_SUCCESS;
}
