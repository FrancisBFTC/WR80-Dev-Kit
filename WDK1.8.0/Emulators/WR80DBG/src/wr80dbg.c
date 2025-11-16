/*
	WR80 Debugger Executable
	Created by Wender Francis (KiddieOS.Community)
	Date: 02/09/2025

*/

#include "../WR80DBG_private.h"
#include "../../../Libraries/WR80DBG/wr80dbg.h"

int main(int argc, char *argv[]) {
	EnableANSI();		// Windows compatibility
	
	if(argc == 1){
		print_version();
		print_help();
		return EXIT_FAILURE;
	}
	
	
	bool debug = false;
	bool listen = false;
	bool bin = false;
	
	char* binary = NULL;
	
	for(int i = 1; i < argc; i++){
		debug = (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) || debug;
		listen = (strcmp(argv[i], "-lm") == 0 || strcmp(argv[i], "--listen-mode") == 0) || listen;
		bin = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) || bin;
		if(binary == NULL && debug)
			binary = argv[i + 1];
	}
	
	if(!listen && !debug){
		perror("Insert debug or listen parameters!\n");
		return EXIT_FAILURE;
	}
	
	if(!listen && debug)
		RunEmulator(binary, bin);
	
	if(!CreateClient(SERVER_PORT))
		return EXIT_FAILURE;
    
    DebugCPUInfo();
    CloseClient();
    
	return EXIT_SUCCESS;
}
