#include "../WR80IMG_private.h"
#include "wr80img.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	if(argc == 1){
		print_version();
		print_usage();
		return EXIT_SUCCESS;
	}
	
	bool source = false;
	bool output = false;
	bool create = false;
	bool format = false;
	bool length = false;
	bool boot = false;
	bool bytes = false;
	bool seek = false;
	bool b2h = false;
	bool h2b = false;
	
	char* srcfile = NULL;
	char* outfile = NULL;
	char* createf = NULL;
	char* lengthf = NULL;
	char* bootf = NULL;
	char* bytesf = NULL;
	char* seekf = NULL;
	
	for(int i = 1; i < argc; i++){
		source = (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--source") == 0) || source;
		output = (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) || output;
		create = (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--create") == 0) || create;
		format = (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) || format;
		length = (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--length") == 0) || length;
		boot = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--boot") == 0) || boot;
		bytes = (strcmp(argv[i], "-bs") == 0 || strcmp(argv[i], "--bytes") == 0) || bytes;
		seek = (strcmp(argv[i], "-sk") == 0 || strcmp(argv[i], "--seek") == 0) || seek;
		b2h = (strcmp(argv[i], "-b2h") == 0) || b2h;
		h2b = (strcmp(argv[i], "-h2b") == 0) || h2b;
		if(srcfile == NULL && source)
			srcfile = argv[++i];
		if(outfile == NULL && output)
			outfile = argv[++i];
		if(createf == NULL && create)
			createf = argv[++i];
		if(lengthf == NULL && length)
			lengthf = argv[++i];
		if(bootf == NULL && boot)
			bootf = argv[++i];
		if(bytesf == NULL && bytes)
			bytesf = argv[++i];
		if(seekf == NULL && seek)
			seekf = argv[++i];
	}
	
	if(create){
		int create_size = 4096;
		if(length){
			char *endptr;
			create_size = strtol(lengthf, &endptr, 10);
			if(*endptr != '\0'){
				perror("Error: Invalid length number.");
				return EXIT_FAILURE;
			}
		}
		create_image(createf, create_size);
		printf("Image '%s' with %d bytes created successfully!\n", createf, create_size);
	}
	
	if(source && output){
		if(format){
			if(boot){
				size_t count = 0, capacity = 100;
				FileEntry *files = malloc(capacity * sizeof(FileEntry));
	    		list_files(srcfile, &files, &count, &capacity);
	    		save_to_binary(outfile, srcfile, bootf, files, count);
	    		free(files);
			}else{
				printf("Error: Specify the boot file.\n");
				return EXIT_FAILURE;
			}
		}else{
			if(!create){
				if(b2h){
					writeHex(srcfile, outfile);
					return EXIT_SUCCESS;
				}else if(h2b){
					writeBin(srcfile, outfile);
					return EXIT_SUCCESS;
				}
			}
			char *endptr;
			int bs = 256;	// standard WR80 sector size
			int sk = 0;
			if(bytes){
				bs = strtol(bytesf, &endptr, 10);
				if(*endptr != '\0'){
					perror("Error: Invalid bytes number.");
					return EXIT_FAILURE;
				}
			}
			if(seek){
				sk = strtol(seekf, &endptr, 10);
				if(*endptr != '\0'){
					perror("Error: Invalid seek number.");
					return EXIT_FAILURE;
				}
			}

			write_binary(srcfile, outfile, sk * bs);
		}
	}else{
		if(!create){
			perror("Error: Specify the -s or -o parameter.\n");
			return EXIT_FAILURE;
		}else{
			if(b2h){
				char* hexfile = changeExtension(createf, ".hex");
				writeHex(createf, hexfile);
				free(hexfile);
			}
		}	
	}
	
	
	return EXIT_SUCCESS;
}
