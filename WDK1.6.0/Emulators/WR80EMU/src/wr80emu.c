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
        printf (" -e   | --emulate <binary_file> : Emulate the binary file\n" \
				" -ed  | --emudbg <binary_file> : Emulate from debugger\n" \
				" -c   | --config : Create standard devices configurations\n" \
				" -rs  | --reset : Reset for standard devices configurations\n" \
				" -lr  | --load-rom <rom_file> : Load from ROM file to ROM emulator\n" \
				" -er  | --exec-rom : Execute from configured ROM file\n\n" \
				"Subparameters for --config flag:\n"
				" -ram  | --ram-addr  <PX:PY>: Define PX:PY format to map the RAM\n" \
				" -vid  | --vid-addr  <PX:PY>: Define PX:PY format to map the VIDEO\n" \
				" -ramd | --ram-data <PX> : Define PX port for RAM data\n" \
				" -vidd | --vid-data <PX> : Define PX port for RGB MONITOR data\n" \
				" -keyd | --key-data <PX> : Define PX port for KEYBOARD data\n" \
				" -ttyd | --tty-data <PX> : Define PX port for TTY MONITOR data\n" \
				" -k    | --keyboard <ON|OFF> : Enable/Disable Keyboard\n" \
				" -m    | --monitor  <ON|OFF> : Enable/Disable Monitor\n" \
				" -vt   | --videotype <TTY|RGB|ANY> : Choose Monitor type (ANY for both)\n" \
				" -rom  | --rom-file <rom_file> : Store a ROM file name\n\n");
		printf("Extra parameters:\n");
		printf (" -d | --debug : Debugging the code from emulator (use -m or -ed before)\n" \
				" -h | --hexdump : Show the hexa code after assembly (use -m or -ed before)\n" \
				" -b | --binary : Assemble the file in binary format\n" \
				" -v | --verbose : Print assembler steps information\n");
				/*" -o | --output <output_file> : Generate the output file (use -m before)\n" \ */
				/*" -w | --write : Write the assembled file with emulating (use -me before)\n" \ */
				/*" -h | --hexdump <binary_file> : Show the hexa code from binary file\n" \ */
        return EXIT_FAILURE;
    }

	bool emulate = false;
	bool config = false;
	bool debug = false;
	bool emudbg = false;
	bool hexdump = false;
	bool output = false;
	bool bin = false;
	
	bool ramhl = false;
	bool vidhl = false;
	bool ramd  = false;
	bool vidd  = false;
	bool keyd  = false;
	bool ttyd  = false;
	bool keyb  = false;
	bool moni  = false;
	bool vidt  = false;
	bool reset = false;
	bool haserror = false;
	
	bool romf = false;
	bool loadrom = false;
	bool execrom = false;
	bool intr = false;
	
	char* binary = NULL;
	char* ramhl_str = NULL;
	char* vidhl_str = NULL;
	char* ramd_str = NULL;
	char* vidd_str = NULL;
	char* keyd_str = NULL;
	char* ttyd_str = NULL;
	char* keyb_str = NULL;
	char* moni_str = NULL;
	char* vidt_str = NULL;
	
	char* romf_str = NULL;
	char* loadr_str = NULL;
	char* intr_str = NULL;
	unsigned char *memory = NULL;
	
    int size = -1;
	
	for(int i = 1; i < argc; i++){
		emulate = (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--emulate") == 0) || emulate;
		config = (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) || config;
		emudbg = (strcmp(argv[i], "-ed") == 0 || strcmp(argv[i], "--emudbg") == 0) || emudbg;
		hexdump = (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hexdump") == 0) || hexdump;
		bin = (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0) || bin;
		debug = (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) || debug;
		ramhl = (strcmp(argv[i], "-ram") == 0 || strcmp(argv[i], "--ram-addr") == 0) || ramhl;
		vidhl = (strcmp(argv[i], "-vid") == 0 || strcmp(argv[i], "--vid-addr") == 0) || vidhl;
		ramd = (strcmp(argv[i], "-ramd") == 0 || strcmp(argv[i], "--ram-data") == 0) || ramd;
		vidd = (strcmp(argv[i], "-vidd") == 0 || strcmp(argv[i], "--vid-data") == 0) || vidd;
		keyd = (strcmp(argv[i], "-keyd") == 0 || strcmp(argv[i], "--key-data") == 0) || keyd;
		ttyd = (strcmp(argv[i], "-ttyd") == 0 || strcmp(argv[i], "--tty-data") == 0) || ttyd;
		keyb = (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--keyboard") == 0) || keyb;
		moni = (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--monitor") == 0) || moni;
		vidt = (strcmp(argv[i], "-vt") == 0 || strcmp(argv[i], "--videotype") == 0) || vidt;
		reset = (strcmp(argv[i], "-rs") == 0 || strcmp(argv[i], "--reset") == 0) || reset;
		romf = (strcmp(argv[i], "-rom") == 0 || strcmp(argv[i], "--rom-file") == 0) || romf;
		loadrom = (strcmp(argv[i], "-lr") == 0 || strcmp(argv[i], "--load-rom") == 0) || loadrom;
		execrom = (strcmp(argv[i], "-er") == 0 || strcmp(argv[i], "--exec-rom") == 0) || execrom;
		intr = (strcmp(argv[i], "-int") == 0 || strcmp(argv[i], "--interrupt") == 0) || intr;
		if(binary == NULL && (emulate || emudbg)) binary = argv[i + 1];
		if(ramhl_str == NULL && ramhl) ramhl_str = argv[i + 1];
		if(vidhl_str == NULL && vidhl) vidhl_str = argv[i + 1];
		if(ramd_str == NULL && ramd) ramd_str = argv[i + 1];
		if(vidd_str == NULL && vidd) vidd_str = argv[i + 1];
		if(keyd_str == NULL && keyd) keyd_str = argv[i + 1];
		if(ttyd_str == NULL && ttyd) ttyd_str = argv[i + 1];
		if(keyb_str == NULL && keyb) keyb_str = argv[i + 1];
		if(moni_str == NULL && moni) moni_str = argv[i + 1];
		if(vidt_str == NULL && vidt) vidt_str = argv[i + 1];
		if(romf_str == NULL && romf) romf_str = argv[i + 1];
		if(loadr_str == NULL && loadrom) loadr_str = argv[i + 1];
		if(intr_str == NULL && intr) intr_str = argv[i + 1];
	}
	
	if(reset){
		save_config("config.dat");
		return EXIT_SUCCESS;
	}
		
	load_config("config.dat");
	
	for(int i = 0; i < _IERR; i++)
		printf("I%d = %d\n", i, devs.intr[i]);
	
	if(config){
		char p1[64], p2[64];
		if(ramhl){
        	haserror = sscanf(ramhl_str, " %63[^:]:%63s", p1, p2) != 2 || haserror;
        	if(!haserror){
        		devs.ram_h = parse_port(p1);
        		devs.ram_l = parse_port(p2);
			}
		}
		if(vidhl){
			haserror = sscanf(vidhl_str, " %63[^:]:%63s", p1, p2) != 2 || haserror;
        	if(!haserror){
        		devs.vid_h = parse_port(p1);
        		devs.vid_l = parse_port(p2);
			}
		}
		if(ramd) devs.ram_d = parse_port(ramd_str);
		if(vidd) devs.vid_d = parse_port(vidd_str);
		if(keyd) devs.key_d = parse_port(keyd_str);
		if(ttyd) devs.tty_d = parse_port(ttyd_str);
		if(keyb) devs.keyboard = (strcasecmp(keyb_str, "ON") == 0);
		if(moni) devs.monitor = (strcasecmp(moni_str, "ON") == 0);
		
		haserror = keyb && !devs.keyboard && (strcasecmp(keyb_str, "OFF") != 0) || haserror;
		haserror = moni && !devs.monitor && (strcasecmp(moni_str, "OFF") != 0) || haserror;
		
		if(vidt){
			devs.tty = devs.rgb = (strcasecmp(vidt_str, "ANY") == 0);
			devs.tty = (strcasecmp(vidt_str, "TTY") == 0) || devs.tty;
            devs.rgb = (strcasecmp(vidt_str, "RGB") == 0) || devs.rgb;
            haserror = (!devs.tty && !devs.rgb) || haserror;
		}
		
		if(romf) strcpy(devs.romf, romf_str);
		if(intr){
			haserror = sscanf(intr_str, " %63[^:]:%63s", p1, p2) != 2 || haserror;
			int devnum = parse_hardware(p2);
        	if(!haserror)
        		devs.intr[parse_intr(p1)] = parse_hardware(p2);
		}
		
		if(haserror){
			printf("Error in one of the arguments - Define ON, OFF, PX or PX:PY format (See HELP)");
			return EXIT_FAILURE;
		}
		save_config("config.dat");
		return EXIT_SUCCESS;	
	}
	
	if(loadrom){
		int loaded = (bin) ? load_bin(loadr_str, &rom) : load_hex(loadr_str, &rom);
		if(loaded < 0){
			printf("The ROM '%s' was not found!\n", loadr_str);
			return EXIT_FAILURE;
		}
		
		if(!(emulate || emudbg)){
			// Auto Loading
			memory = malloc(memory_size);
			clear_ram(memory);
			memcpy(memory, rom, loaded);
			emulate = true;
			size = loaded;
		}else{
			size = (bin) ? load_bin(binary, &memory) : load_hex(binary, &memory);
		}
		strcpy(devs.romf, loadr_str);
	}else{
		int loaded = -1;
		if(devs.romf[0]){
			loaded = (bin) ? load_bin(devs.romf, &rom) : load_hex(devs.romf, &rom);
			if(loaded < 0){
				printf("The ROM '%s' was not found!\n", devs.romf);
				return EXIT_FAILURE;
			}	
		}
		
		if(execrom){
			if(!devs.romf[0]){
				printf("Error: Configure ROM in the config.dat\n");
				return EXIT_FAILURE;
			}
			// Auto Loading
			memory = malloc(memory_size);
			clear_ram(memory);
			memcpy(memory, rom, loaded);
			emulate = true;
			size = loaded;
		}else if(emulate || emudbg){
			size = (bin) ? load_bin(binary, &memory) : load_hex(binary, &memory);
		}else{
			printf("Invalid Argument - See the HELP!\n");
			return EXIT_FAILURE;
		}
	}
	
	if(size > 0 && hexdump)
		hex_dump(memory, 0x000, size);
		
	activate_debug(debug || emudbg);
	
	if((emulate || emudbg) && size != -1){
		bool emulated = emulate_buffer(memory, size, emudbg);
		free(memory);
		memory = NULL;
		
		if(devs.romf[0]){
			write_bin(devs.romf, rom, size);
			free(rom);
			rom = NULL;
		}
	}
    
	return 0;
}
