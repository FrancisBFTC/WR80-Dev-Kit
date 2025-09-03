#include <stdio.h>
#include <stdlib.h>

/*
	WR80 Emulator Executable
	Created by Wender Francis (KiddieOS.Community)
	Date: 30/08/2025

*/

#include "../wr80emu_private.h"
#include "wr80emu.h"

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
				" -v | --verbose : Print assembler steps information\n");
        return EXIT_FAILURE;
    }

	bool emulate = false;
	bool debug = false;
	bool hexdump = false;
	bool output = false;
	bool bin = false;
	
	char* binary = NULL;
	unsigned char *memory = NULL;
	
    int size = -1;
	
	for(int i = 1; i < argc; i++){
		emulate = (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--emulate") == 0) || emulate;
		hexdump = (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hexdump") == 0) || hexdump;
		bin = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) || bin;
		debug = (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) || debug;
		if(binary == NULL && emulate)
			binary = argv[i + 1];
	}
	
	size = (bin) ? load_bin(binary, &memory) : load_hex(binary, &memory);
	
	if(size > 0 && hexdump)
		hex_dump(memory, 0x000, size);
		
	activate_debug(debug);
	
	if(emulate && size != -1){
		bool emulated = emulate_buffer(memory, size);
		free(memory);
		memory = NULL;
	}
    
	return 0;
}
