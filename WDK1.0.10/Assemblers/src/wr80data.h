/*
	WR80 Assembler data Library
	Created by Wender Francis (KiddieOS.Community)
	Date: 20/08/2025

*/

#ifndef __WR80DATA_H__
#define __WR80DATA_H__

// FUNCTIONS PROTOTYPE FOR PREPROCESSOR AND ASSEMBLER
// -----------------------------------------------------------------------------
bool tokenizer(void);
bool parser(void);
bool parse_addressing(int);
bool generator(void);
bool dcb_process(void);

void format_line(void);
void format_operand(void);
void reset_states(void);

bool preprocess_file(const char*, bool);
bool assemble_file(const char*, unsigned char **, bool);
char *load_file_to_buffer(const char*);
bool preprocess_buffer(const char*, bool);
bool assemble_buffer(const char*, unsigned char**, bool);
void proc_define(void);
void proc_dcb(void);
void proc_org(void);
void proc_include(void);
void (*func_ptr)();

void printerr(const char*);
void printwarn(const char*);
bool recursive_def(char*);
char* replace(char*, const char*, const char*);
void hex_dump(unsigned char* code);
int replace_name(char* name);
// -----------------------------------------------------------------------------

#define MAX_LINE_LENGTH 1024		// MAX LENGTH OF THE LINES
#define MEMORY_EMULATOR 65535		// MAX LENGTH OF THE EMULATOR

// ADRESSING TYPES
// -----------------------------------------------------
#define IMP		0x00
#define IMM 	0x01
#define REG 	0x02
#define AB		0x04
#define REL 	0x08
// -----------------------------------------------------

// VARIABLES STATES FOR PREPROCESSOR AND ASSEMBLER
// -----------------------------------------------------

// Token Strings and Code Buffers
// -----------------------------------------------------
unsigned char *memory = NULL;
unsigned char *code_address = NULL;
char *token;
char *directive;
char *mnemonic;
char *operand;
char *label;
char *endptr;
char *currentfile;

char line[MAX_LINE_LENGTH];
char dest[50];
// -----------------------------------------------------

// Integer values
// -----------------------------------------------------
int linenum = 1;
int number;
int len;
int bit_shift;
int mnemonic_index = 0;
int isDefinition = 0;
int code_index = 0;
int dcb_index = 0;
int reg_index = 0;
// -----------------------------------------------------

// Assembler boolean states
// -----------------------------------------------------
bool isDirective = false;
bool isMnemonic = false;
bool isHexadecimal = false;
bool listInitialized = false;
bool isLabel = false;
bool isRelative = false;
bool isAllocator = false;
bool isOrg = false;
bool isInclude = false;
bool isHigh = false;
bool isDecimal = false;
bool isReferenced = false;
bool toIgnore = false;
bool isLineComment = false;
bool directive_error = false;

bool syntax_6502 = false;
bool syntax_PIC = false;
bool syntax_Intel = false;
bool syntax_GAS = false;

bool isBuffer = false;
bool isVerbose = false;
// -----------------------------------------------------

// List structures for the preprocessor
// -----------------------------------------------------
DefineList *define_list;
DcbList *dcb_list;
LabelList *label_list;
RefsAddr* curr_refer = NULL;
// -----------------------------------------------------

// -----------------------------------------------------

// WR80's Assembly Mnemonics Vector
// -----------------------------------------------------
#define MNEMONICS_SIZE 	49
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
	
	// Allocator Commands
	"DCB",
	".BYTE",
	"DB",
	"DW",
	"ORG",
	"INCLUDE"
};
// -----------------------------------------------------

// WR80's User and Port Registers
// -----------------------------------------------------
const char* user_registers[] = {
	"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
	"%R0", "%R1", "%R2", "%R3", "%R4", "%R5", "%R6", "%R7"
};

const char* port_registers[] = {
	"P0", "P1", "P2", "P3", "P4", "P5", "P6", "P7",
	"%P0", "%P1", "%P2", "%P3", "%P4", "%P5", "%P6", "%P7"
};
// -----------------------------------------------------

// WR80's opcodes (ISA)
// -----------------------------------------------------
const unsigned char opcodes[] = {
	0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0,
	0xC0, 0xD0, 0xE0, 0xF0, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x28, 0x29, 0x2A,
	0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x38, 0x48, 0x58
};
// -----------------------------------------------------

// WR80's opcode addressing type
// -----------------------------------------------------
const unsigned short addressing[] = {
	REG, REG, REG, REG,	REG, REG, 			// Logical and Aritmethic Instructions
	IMM, REG, REG, REG,						// Movements Instructions
	IMM, IMM, 								// Shift Instructions
	REG, REL, REL, REL,						// Comparation and Jump Instructions
	IMP, IMP, IMP, IMP, IMP, IMP, IMP, IMP,	// Enabling and Cleaning Instructions
	IMP, IMP, IMP, IMP, IMP, IMP, IMP, IMP,	// Stack Instructions v1
	IMP, IMP, IMP, IMP, IMP, IMP, IMP, IMP, // Stack Instructions v2
	REG, REG, REL, IMP, IMP, IMP, IMP, AB 	// Stack Instructions v3
};
// -----------------------------------------------------

// Preprocessor basic directives
// -----------------------------------------------------
#define DIRECTIVES_SIZE 	2
const char* directives[] = {
	"DEFINE",
	"INCLUDE"
};
// -----------------------------------------------------

// Preprocessor Execution vector for directives
// -----------------------------------------------------
int* process[] = {
	(int*)proc_define, (int*)proc_include
};

// -----------------------------------------------------

#endif
