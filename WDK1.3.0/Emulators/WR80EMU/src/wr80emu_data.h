#ifndef __WR80EMU_DATA_H__
#define __WR80EMU_DATA_H__

#define MAX_MEMORY 4096  // WR80 maximum memory size (4 KB)
#define EXTENSION_BIT 3
#define EXTENSION 	(1 << EXTENSION_BIT)
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// Emulator Functions Prototype
// ---------------------------------------------
int load_hex(const char*, unsigned char**);
int load_bin(const char*, unsigned char**);
void hex_dump(unsigned char*, uint16_t, int);
char* hexdump_dbg(unsigned char*, uint16_t, int);
bool emulate_buffer(unsigned char*, int, bool);
int16_t sign_extend(uint16_t);
void print_bin4(uint8_t);
void clear_ram(unsigned char*);
void activate_debug(bool);
void debug_process(bool);
char* get_cpu_info();
int CreateServer(int);
int GetConnection(bool);
void CloseServer(void);
int execute_command(const char*);

// 16 classical instruction functions
void proc_and(void);
void proc_or(void);
void proc_not(void);
void proc_xor(void);

void proc_add(void);
void proc_sub(void);

void proc_st(void);
void proc_ld(void);
void proc_in(void);
void proc_out(void);

void proc_shr(void);
void proc_shl(void);

void proc_bt(void);
void proc_jc(void);
void proc_jz(void);
void proc_jp(void);

// 22 extended instruction functions
void proc_ei(void);
void proc_di(void);
void proc_ed(void);
void proc_dd(void);
void proc_ec(void);
void proc_dc(void);
void proc_cdr(void);
void proc_clr(void);

void proc_pushb(void);
void proc_popb(void);
void proc_pushs(void);
void proc_pops(void);
void proc_sbp(void);
void proc_abp(void);
void proc_ssp(void);
void proc_iret(void);

void proc_pushd(void);
void proc_popd(void);
void proc_sbw(void);
void proc_scr(void);
void proc_scs(void);
void proc_pusha(void);
void proc_popa(void);
void proc_ret(void);

void proc_push(void);
void proc_pop(void);
void proc_callpos(void);
void proc_callneg(void);
void proc_call(uint8_t);

void proc_mul(void);
void proc_div(void);
void proc_stl(void);
void proc_std(void);

void proc_inc(void);
void proc_dec(void);
void proc_idc(void);

// ---------------------------------------------
  
size_t memory_size = MAX_MEMORY;
void (*cpu_operation)();
uint8_t curr_opcode;
uint8_t next_opcode;
unsigned char* ram = NULL;
unsigned char* rom = NULL;
unsigned char* stack = NULL;
unsigned char* breaks = NULL;
bool di_state = false;
bool debug_mode = false;
bool isExtension = false;
bool connected = false;
bool exec_mode = false;
int mnemonic = 0;
int clr = 0;

WSADATA wsa;
SOCKET server_fd, client_fd;
struct sockaddr_in address, client;

#ifdef WR80VM_PRIVATE_H
	static CRITICAL_SECTION cs;
	
	// Estrutura para passar argumentos
	typedef struct {
	    unsigned char* buf;
	    int size;
	    bool flag;
	    char filename[MAX_PATH];
	} EmuArgs;
	
	EmuArgs args;
#endif

#define _P0		0x00
#define _P1		0x01
#define _P2		0x02
#define _P3		0x03
#define _P4		0x04
#define _P5		0x05
#define _P6		0x06
#define _P7		0x07

#define _I0 	0
#define _I1		1
#define _I2 	2
#define _I3 	3
#define _IERR 	4

struct Devices {
	uint8_t ram_h;
	uint8_t ram_l;
	uint8_t vid_h;
	uint8_t vid_l;
	uint8_t ram_d;
	uint8_t key_d;
	uint8_t vid_d;
	uint8_t tty_d;
	bool keyboard;
	bool monitor;
	bool tty;
	bool rgb;
	bool ints;
	char romf[256];
	uint8_t intr[4];
};

struct Devices devs = {
    .ram_h = _P0,
    .ram_l = _P1,
    .vid_h = _P4,
    .vid_l = _P5,
    .ram_d = _P2,
    .key_d = _P3,
    .vid_d = _P6,
    .tty_d = _P3,
    .keyboard = true,
    .monitor = true,
    .tty = true,
    .rgb = true,
    .ints = false,
    .romf = {'\0'},
    .intr = {_IERR, _IERR, _IERR, _IERR}
};

// THE 8 CPU PRIVATE ACCESS REGISTERS
// ---------------------------------------------
uint8_t IR = 0;
uint8_t RR = 0;
uint16_t PC = 0;		// only 12 bits are used.
uint16_t OFFSET = 0;	// only 12 bits are used.
uint8_t STHR = 0;
uint8_t STLR = 0;
uint16_t ISR = 0;		// only 12 bits are used.
uint16_t P2I = 0;
// ---------------------------------------------


// THE 20 PUBLIC USER ACCESS REGISTERS
// *********************************************

// THE 4 IMPLICIT ACCESS REGISTERS
uint16_t SP = 0xFFF;	// only 12 bits are used.
uint16_t BP = 0xFFF;	// only 12 bits are used.
uint8_t DR = 0;
uint8_t SR = 0;			// only 4 bits are used.

// THE 16 EXPLICIT ACCESS REGISTERS
uint8_t RX[8]; 	// THE 8 USER REGISTERS
uint8_t PX[8]; 	// THE 8 PORT REGISTERS

// ADDICTIONALS REGISTERS FOR INCREMENT CONFIG
uint8_t IDCM = 0; 		// IDC Map
uint8_t IDCHR = 0; 		// IDC High Register
uint8_t IDCLR = 0; 		// IDC Low Register
// *********************************************

#define OPCODES_SIZE 51
// WR80's opcodes (ISA)
// -----------------------------------------------------
const unsigned char opcodes[] = {
	0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0,
	0xC0, 0xD0, 0xE0, 0xF0, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x28, 0x29, 0x2A,
	0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x38, 0x48, 0x58, 0x78, 0x88, 0x98, 0xA8,
	0xB8, 0xB9, 0xBA, 0xBB
};
// -----------------------------------------------------

// Preprocessor Execution vector for directives
// -----------------------------------------------------
int* process[] = {
	(int*)proc_and, (int*)proc_or, 	(int*)proc_not, (int*)proc_xor,
	(int*)proc_add, (int*)proc_sub, (int*)proc_st, 	(int*)proc_ld,
	(int*)proc_in,	(int*)proc_out, (int*)proc_shr, (int*)proc_shl,
	(int*)proc_bt, 	(int*)proc_jc,	(int*)proc_jz, 	(int*)proc_jp,
	
	(int*)proc_ei, 	(int*)proc_di, 	(int*)proc_ed,	(int*)proc_dd,
	(int*)proc_ec, 	(int*)proc_dc, 	(int*)proc_cdr, (int*)proc_clr,
	
	(int*)proc_pushb, (int*)proc_popb, (int*)proc_pushs, (int*)proc_pops,
	(int*)proc_sbp,   (int*)proc_abp,  (int*)proc_ssp,	 (int*)proc_iret,
	(int*)proc_pushd, (int*)proc_popd, (int*)proc_sbw,	 (int*)proc_scr,
	
 	(int*)proc_scs,  (int*)proc_pusha, (int*)proc_popa,    (int*)proc_ret,   
	(int*)proc_push, (int*)proc_pop,   (int*)proc_callpos, (int*)proc_callneg,
	
	(int*)proc_mul,	 (int*)proc_div,   (int*)proc_stl, 	   (int*)proc_std,
	
	(int*)proc_inc,	 (int*)proc_dec,   (int*)proc_idc
	
};

// -----------------------------------------------------

// WR80's Assembly Mnemonics Vector
// -----------------------------------------------------
#define MNEMONICS_SIZE 	51
const char* mnemonics[] = {
	// Logical Instructions
	"AND",
	"OR",
	"NOT",
	"XOR",
	
	//Aritmethic Instructions
	"ADD",
	"SUB",
	
	// Move Instructions
	"ST",
	"LD",
	"IN",
	"OUT",
	
	// Shift Instructions
	"SHR",
	"SHL",
	
	// Comparators
	"BT",
	
	// Jump Instructions
	"JC",
	"JZ",
	"JP",
	
	// Enabling/Flag Instructions (Processor Status - No operands)
	"EI",
	"DI",
	"ED",
	"DD",
	"EC",
	"DC",
	
	// Cleaning Instructions
	"CDR",
	"CLR",
	
	// Stack Instructions v1 (BP and SP)
	"PUSHB",
	"POPB",
	"PUSHS",
	"POPS",
	"SBP",
	"ABP",
	"SSP",
	"IRET",
	
	//Stack Instructions v2 (ACC)
	"PUSHD",
	"POPD",
	"SBW",
	"SCR",
	"SCS",
	"PUSHA",
	"POPA",
	"RET",
	
	// Stack Instructions v3 (Registers and Relative)
	"PUSH",
	"POP",
	"CALL",
	"CALL",
	
	// Extended instructions for MUL,DIV,STL and STD
	"MUL",
	"DIV",
	"STL",
	"STD",
	
	// Increment instructions
	"INCR",
	"DECR",
	"IDC"
};
// -----------------------------------------------------

// WR80's User and Port Registers
// -----------------------------------------------------
const char* user_registers[] = {
	"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7"
};

const char* port_registers[] = {
	"P0", "P1", "P2", "P3", "P4", "P5", "P6", "P7"
};
// -----------------------------------------------------

const char* hardware[] = {
	"KEYBOARD",
	"MOUSE",
	"TIMER",
	"UNKNOWN"
};

#endif
