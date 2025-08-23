/*
	WR80 Assembler Functions Library
	Created by Wender Francis (KiddieOS.Community)
	Date: 20/08/2025

*/

#ifndef __WR80ASM_H__
#define __WR80ASM_H__

/*
	STANDARD LIBC INCLUDES CONDITIONS
	You can include those libraries before the wr80asm.h library
	'cause this ifndef conditions, however, it's not necessary include
	the wr80list.h and wr80data.h outside, except for new assembler versions.
*/
// -----------------------------------------------------------------------------
#ifndef _INC_STDIO
#include <stdio.h>
#endif
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif
#ifndef _INC_STRING
#include <string.h>
#endif
#ifndef _STDBOOL_H
#include <stdbool.h>
#endif
#ifndef _INC_CTYPE
#include <ctype.h>
#endif
#ifndef _MATH_H_
#include <math.h>
#endif

#include "wr80list.h"	// WR80 list Structures for labels, defines and DBs
#include "wr80data.h"	// WR80 Variables, Structs and Data for Assembler
// -----------------------------------------------------------------------------

// FUNCTIONS TO WARNING AND ERROR MESSAGES
// -----------------------------------------------------------------------------
void printerr(const char* msg){
	if(!isBuffer)
		printf("%s -> Error: Syntax error at line %d - %s\n", currentfile, linenum, msg);
	else
		printf("Error: Syntax error at line %d - %s\n", linenum, msg);
}

void printwarn(const char* msg){
	if(!isBuffer)
		printf("%s -> Warning: %s at line %d.\n", currentfile, msg, linenum);
	else
		printf("Warning: %s at line %d.\n", msg, linenum);
}

void error(const char* msg){
	if(!isBuffer)
		printf("%s -> %s: error at line %d - %s\n", currentfile, mnemonic, linenum, msg);
	else
		printf("%s: error at line %d - %s\n", mnemonic, linenum, msg);
}
// -----------------------------------------------------------------------------

//  FUNCTION PROTOTYPES
void hex_dump(unsigned char* code);
int replace_name(char* name);
// -----------------------------------------------------------------------------

// FUNCTIONS TO FORMAT LINE AND OPERANDS
// -----------------------------------------------------------------------------
// format_line: This function convert tab in spaces, lowercase to uppercase and
// clear the lines break char
void format_line(){
	int pos = strcspn(line, "\n");
	if(isBuffer && line[pos-1] == 0x0D){
		line[pos] = '\0';
		line[pos-1] = '\0';	
	}else{
		line[pos] = '\0';
	}
	bool isQuote = false;
	for(int i = 0; i < strlen(line); i++){
		isQuote = line[i] == '"' ^ isQuote;
		if(line[i] == 0x09)
			line[i] = 0x20;
		else if(line[i] > 0x60 && line[i] < 0x7B && !isQuote)
				line[i] -= 0x20;
	}
}

// format_operand: This function concat tokens in operands
void format_operand(){
	strcat(operand, token);
    token = strtok(NULL, " ");
}
// -----------------------------------------------------------------------------

// FUNCTIONS TO PROCESS DIRECTIVES IN PREPROCESSOR
// **********************************************************************************

// proc_org: Organize and Allocate memory intervals filling with Zeros (alignment)
// -----------------------------------------------------------------------------
void proc_org(){
	if(code_index <= number){
		for(; code_index < number; code_index++){
			code_address[code_index] = 0x00;
		}	
	}else{
		printwarn("Is not possible to organize this memory location");
		return;	
	}
}
// -----------------------------------------------------------------------------

// proc_dcb: Allocate data byte or data word
// -----------------------------------------------------------------------------
void proc_dcb(){
	token = strtok(NULL, "\n");
	operand = token;
	
	int i = 0;
	int length = 0;
	unsigned char* value = malloc(strlen(token) + 1);
	bool isHexa = false;
	bool isHexa2 = false;
	bool isNum = false;
	bool isLowByte = false;
	bool isHighByte = false;
	bool isBitIsolate = false;
	while(token[i] != '\0'){
		if(token[i] == '"'){
			i++;
			while(token[i] != '"' && token[i] != '\0'){
				//char num = token[i];
				//value = realloc(value, length+1);
				value[length++] = token[i++];
			}
			if(token[i] == '"') i++;
			continue;
		}
		isLowByte = token[i] == '<';
		isHighByte = token[i] == '>';
		isHigh = isHighByte;
		isBitIsolate = (isLowByte || isHighByte);
		if(isBitIsolate) ++i;
		isHexa2 = token[i] == '0' && token[i+1] == 'X' || token[i] == 'H' && token[i+1] == '\'';
		isHexa = token[i] == '$' || isHexa2;
		isNum = token[i] >= 0x30 && token[i] <= 0x39;
		if(!isHexa && !isNum && token[i] != ',' && token[i] != '"' && token[i] != ' '){
			int namelen = strcspn(&token[i], ",");
			char name[namelen+1];
			memcpy(name, &token[i], namelen);
			name[namelen] = 0;
			
			dcb_index = length;
			
			if(replace_name(name) != -1){
				if(isBitIsolate) --i;
				continue;
			}else{
				directive_error = true;
				printerr("Invalid or Undefined value - use number or define this name");
				return;	
			}
		}
		if(isHexa || isNum){
			char val[10] = {0};
			int j = 0;
			if(isHexa) i = i + 1;
			if(isHexa2) i = i + 1;
			
			while(token[i] != ',' && token[i] != NULL)
				val[j++] = token[i++];
			val[j] = 0;
			if(token[i-1] == '\'')
				val[--j] = 0;
			
			bool isDW = mnemonic_index == 46;
			int base = (isHexa) ? 16 : 10;
			int num = strtol(val, &endptr, base);
			if(((j > 2 && isHexa) || (num > 255 && !isHexa)) && !isBitIsolate && !isDW)
				printwarn("DCB byte is larger than 8-bit. Only low byte will be considered");
		
			//value = realloc(value, length+1);
			
			if(isDW){
				value[length++] = (num & 0xFF);
				value[length++] = (num & 0x0F00) >> 8;
			}else{
				value[length++] = (isHighByte) ? (num & 0xFF00) >> 8 : num & 0xFF;
			}
			
			if (*endptr != '\0') {
				directive_error = true;
				printerr("Cannot parse the hexa number");
				return;
			}
			continue;
		}
		i++;
	}
	dcb_list = insertdcb(dcb_list, linenum, length, value);
	free(value);
}
// -----------------------------------------------------------------------------

// proc_define: Store definitions in lists for replacement
// -----------------------------------------------------------------------------
void proc_define(){
	int pos = 1;
	char* name = NULL;
	char* value = NULL;
	
	while(token != NULL){
		token = strtok(NULL, " ");
		switch(pos){
			case 1:	name = token;
					break;
			case 2: value = token;
					break;
			default: if(token != NULL){
						printerr("Invalid defined token");
						directive_error = true;
					 }
		}
		if(directive_error)
			return;
			
		pos++;
	}
	
	bool isNum = name[0] > 0x30 && name[0] <= 0x39;
	bool isSymbol = false;
	for(int i = 0; i < strlen(name); i++)
		isSymbol = name[i] == '#' || name[i] == '$' || name[i] == ':' || isSymbol;
		
	if(isNum || isSymbol){
		printerr("Invalid defined name");
		directive_error = true;
		return;
	}
	
	DefineList* def = getdef(define_list, name);
	if(def != NULL){
		if(!isBuffer)
			printf("%s -> Error: This name '%s' at line %d is already defined at line %d.", currentfile, def->name, linenum, def->line);
		else
			printf("Error: This name '%s' at line %d is already defined at line %d.", def->name, linenum, def->line);
		directive_error = true;
		return;
	}else{
		LabelList* lab = getLabelByName(label_list, name);
		if(lab != NULL){
			if(!isBuffer)
				printf("%s -> Error: This label '%s' at line %d is already defined at line %d.", currentfile, lab->name, linenum, lab->line);
			else
				printf("Error: This label '%s' at line %d is already defined at line %d.", lab->name, linenum, lab->line);
			directive_error = true;
			return;
		}
	}
		
	if(value[0] == '#'){
		printerr("Invalid defined value - remove '#'");
		directive_error = true;
		return;
	}else if(value[0] == '$'){
		strtol(&value[1], &endptr, 16);
		if (*endptr != '\0') {
			printerr("Invalid defined value - hexa error");
			directive_error = true;
			return;
		}
	}else{
		if(!recursive_def(value)) {
			define_list = insertdef(define_list, linenum, name, NULL, value);
			return;
		}
	}
	define_list = insertdef(define_list, linenum, name, value, NULL);
}
// -----------------------------------------------------------------------------

// proc_include: Preprocess and assemble file's inclusions
// -----------------------------------------------------------------------------
void proc_include(){
	char file_name[128] = {0};
	token = strtok(NULL, "\"");

	if (token == NULL) {
		fprintf(stderr, "Error: empty file name in include directive.\n");
		directive_error = true;
		return;
	}

	strncpy(file_name, token, sizeof(file_name) - 1);

	int linetemp = linenum;
	char* filetemp = currentfile;
	bool mounted = false;
	if(!isInclude){
		if(isBuffer){
			char *source_code = load_file_to_buffer(file_name);
			mounted = preprocess_buffer(source_code, isVerbose);
			free(source_code);
		}else{
			mounted = preprocess_file(file_name, isVerbose);
		}
	}else{
		unsigned char* machinecode = NULL;
		if(isBuffer){
			char *source_code = load_file_to_buffer(file_name);
			mounted = assemble_buffer(source_code, &machinecode, isVerbose);
			free(source_code);
		}else{
			mounted = assemble_file(file_name, &machinecode, isVerbose);
		}
		if(isVerbose) {
			hex_dump(machinecode);
			printf("\n");	
		}
		isInclude = false;
	}
	linenum = linetemp;
	currentfile = filetemp;
	
	if (!mounted) {
		fprintf(stderr, "Error: error in assemble the included file: %s\n", file_name);
		directive_error = true;
	}
}
// -----------------------------------------------------------------------------

// dcb_process: allocate physically bytes in code memory by DCB commands
// -----------------------------------------------------------------------------
bool dcb_process(){
	if(isAllocator){
		DcbList* dcb = getdcb(dcb_list, linenum);
		memcpy(&code_address[code_index], dcb->value, dcb->length);
		code_index += dcb->length;
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------------

// **********************************************************************************

// FUNCTIONS TO DEFINITIONS READING
// **********************************************************************************

// recursive_def: definitions recursive reading fetching value defined
// -----------------------------------------------------------------------------
bool recursive_def(char* value){
	int base = (value[0] != '$' && (value[0] != '0' && value[1] != 'X') && (value[0] != 'H' && value[1] != '\'')) ? 10 : 16;
	int index = (base == 16 && value[0] == '$') ? (base >> 4) : 2;
	index = (base == 10) ? 0 : index;
	if(value[strlen(value) - 1] == '\'')
		value[strlen(value) - 1] = '\0';
	strtol(&value[index], &endptr, base);
	if (*endptr != '\0') {
		DefineList* definition = getdef(define_list, value);
		if(definition == NULL)
			return false;
		strcpy(value, definition->value);
		return recursive_def(value);
	}
	return true;	
}
// -----------------------------------------------------------------------------

// replace: Replace old_substr to new_substr in token
// -----------------------------------------------------------------------------
char* replace(char* token, const char* old_substr, const char* new_substr){
	char* pos = strstr(token, old_substr);
	if(pos != NULL){
		int old_len = strlen(old_substr);
		int new_len = strlen(new_substr);
		int buf_len = strlen(token) + (new_len - old_len);
		char* buffer = (char*) malloc(buf_len);
		int prefix_len = pos - token;
		strncpy(buffer, token, prefix_len);
		buffer[prefix_len] = '\0';
		
		strcat(buffer, new_substr);
		strcat(buffer, pos + old_len);
		buffer[buf_len] = '\0';
		
		return buffer;
	}
	return token;
}
// -----------------------------------------------------------------------------

// replace_name: replace de defined name to the value
// -----------------------------------------------------------------------------
int replace_name(char* name){
	char* value;
	char str[6];
	DefineList* definition = getdef(define_list, name);
	
	if(definition == NULL){
		LabelList* label = getLabelByName(label_list, name);
		if(label != NULL){
			bool isRel = (addressing[mnemonic_index] & REL) == REL;
			bool isIMM = isAllocator;
			bool isDW = mnemonic_index == 46;
			if(isRel)
				isRelative = true;
			else
				isLabel = true;
				
			if(label->addr == 0xFFFF){
				int addr_index = code_index + dcb_index;
				label->refs = insertaddr(label->refs, addr_index, isRel, isIMM, isHigh, isDW);
				curr_refer = label->refs;
				
			}
			sprintf(str, "%d", label->addr);
			value = str;
				
		}else{
			printerr("undefined value");
			return -1;	
		}
	}else{
		if (definition->refs[0] == 0){
			value = definition->value;
		}else{
			value = definition->refs;
			token = replace(token, name, value);
			return replace_name(value);
		}
	}
	token = replace(token, name, value);
	return 1;	
}
// -----------------------------------------------------------------------------

// check_definition: Verify if the operand has defined name and replace it
// -----------------------------------------------------------------------------
int check_definition(){
	int index = 0;
	if(token[index] == '$') 
		index += 1;
	else if((token[index] == '0' && token[index+1] == 'X') || (token[index] == 'H' && token[index+1] == '\''))
		index += 2;
		
	int namelen = strcspn(&token[index], "::");
	char name[namelen+1];
	memcpy(name, &token[index], namelen);
	if(name[namelen-1] == '\''){
		name[namelen-1] = '\0';
		namelen -= 1;
	}

	if(name[0] >= 0x30 && name[0] <= 0x39)
		return 0;
		
	name[namelen] = 0;
	strtol(&name[0], &endptr, 10);
	if(*endptr != '\0'){
		strtol(&name[0], &endptr, 16);
		bool possibleHexaError = (!index && (token[index] != '$' 
								&& (token[index] != '0' && token[index+1] != 'X') 
								&& (token[index] != 'H' && token[index+1] != '\'')));
		if(*endptr != '\0' || possibleHexaError){
			return replace_name(name);
		}
			
	}

	return 0;
}
// -----------------------------------------------------------------------------

// **********************************************************************************

// FUNCTIONS TO RETURN MNEMONIC AND DIRECTIVE INDEX
// **********************************************************************************

// get_directive: run the directive process and return its index.
// -----------------------------------------------------------------------------
int get_directive(){
	for(int i = 0; i < DIRECTIVES_SIZE; i++){
		if(strcmp(directives[i], token) == 0){
			func_ptr = (void(*)())process[i];
			func_ptr();
			return i;	
		}	
	}
	return -1;	
}
// -----------------------------------------------------------------------------

// get_mnemonic: compare mnemonic string and return its index
// -----------------------------------------------------------------------------
int get_mnemonic(){
	for(int i = 0; i < MNEMONICS_SIZE; i++){
		if(strcmp(mnemonics[i], token) == 0){
			return i;
		}	
	}
	return -1;
}
// -----------------------------------------------------------------------------

// **********************************************************************************

// FUNCTIONS TO READ LABELS AND CALCULATE IN PREPROCESSOR AND ASSEMBLER
// **********************************************************************************

// get_label: read label and store in list on preprocessor
// -----------------------------------------------------------------------------
bool get_label(int length){
	while(token != NULL){
		int pos = strcspn(&label[0], ":");
		if(label[pos] == ':'){
			token = strtok(NULL, " ");
			if(token != NULL || label[pos+1] != NULL){
				printerr("Invalid label name - incorrect char");
				return false;
			}
							
			label[pos] = '\0';
			strtol(label, &endptr, 10);
							
			if(*endptr == NULL || (label[0] >= 0x30 && label[0] <= 0x39)){
				printerr("Invalid label name - Incorrect format");
				return false;
			}
			for(int i = 0; i < strlen(label); i++){
				bool isCharLower = label[i] > 0x60 && label[i] < 0x7B;
				bool isCharUpper = label[i] > 0x40 && label[i] < 0x5B;
				bool isNum = label[i] >= 0x30 && label[i] <= 0x39;
				bool isAllowerChars = label[i] == '_' || label[i] == '.';
				if(!isCharLower && !isCharUpper && !isNum && !isAllowerChars){
					printerr("Invalid label name - Incorrect char");
					return false;
				}
			}
			
			
			DefineList* def = getdef(define_list, label);
			if(def == NULL){
				LabelList* lab = getLabelByName(label_list, label);
				if(lab == NULL){
					label_list = insertlab(label_list, linenum, label, 0xFFFF);	// 0x0000
					label_list->refs = NULL;
				}else{
					if(!isBuffer)
						printf("%s -> Error: This label '%s' at line %d is already defined at line %d.", currentfile, lab->name, linenum, lab->line);
					else
						printf("Error: This label '%s' at line %d is already defined at line %d.", lab->name, linenum, lab->line);
					return false;
				}
			}else{
				if(!isBuffer)
					printf("%s -> Error: This name '%s' at line %d is already defined at line %d.", currentfile, def->name, linenum, def->line);
				else
					printf("Error: This name '%s' at line %d is already defined at line %d.", def->name, linenum, def->line);
				return false;
			}
							
							
		}else{
			token = strtok(NULL, " ");
			if(token != NULL){
				strcat(label, &token[0]);
				int x = (strlen(label) == length) ? 1 : 0;
				if(strcmp(token-x, ":") != 0){
					printerr("Invalid label name or inexistent instruction");
					return false;
				}
			}else{
				printerr("Invalid label name - missing ':'");
				return false;
			}
		}	
	}
	return true;
}
// -----------------------------------------------------------------------------

// calc_label: calculate the label address on assembler
// -----------------------------------------------------------------------------
bool calc_label(unsigned char *label){
	LabelList* list = getLabelByName(label_list, label);
	
	
	if(list != NULL){
		list->addr = code_index;
		
		if(list->refs != NULL){
			setref(list->refs, code_address, list->addr);	
			freeref(list->refs);
			list->refs = NULL;
		}
						
		toIgnore = true;
		return toIgnore;
	}else{
		printerr("Unknown mnemonic");
		return false;
	}
}
// -----------------------------------------------------------------------------

// **********************************************************************************

// FUNCTIONS TO RUN EACH STEP OF THE ASSEMBLER
// **********************************************************************************

// tokenizer: it's the lexycal analyzer step getting each token
// -----------------------------------------------------------------------------
bool tokenizer(){
	format_line();
	token = strtok(line, " ");
	
	toIgnore = token == NULL || token[0] == ';';
	if(toIgnore) return true;
	int linelen = strlen(line);
	for(int x = 0; x < linelen; x++){
		if(line[x] == ' ')
			continue;
		if(line[x] == 0x0D){
			toIgnore = true;
			return true;	
		}else{
			break;
		}
	}
		
        
	int i = 0;
	int count_tok = 0;
	reset_states();
	isDefinition = 0;
	isDirective = false;
	
    while (token != NULL) {
    	isLineComment = token[0] == ';' || isLineComment;
    	if(isLineComment){
    		token = strtok(NULL, " ");
    		continue;
		}
    	if(count_tok >= 2){
    		format_operand();
        	count_tok++;
        	if(token != NULL)
        		continue;
    		token = operand;
    		isMnemonic = true;
		}
		
		syntax_GAS = token[0] == '%';
		bool isNotRegister = false;
		reg_index = -1;
		for(int i = 0; i < 16; i++){
			if(strcmp(user_registers[i], token) == 0 || strcmp(port_registers[i], token) == 0){
				reg_index = (syntax_GAS) ? i - 8 : i;
				isNotRegister = true;
				break;
			}
		}
		
		if(count_tok > 0 && !isNotRegister && token[0] != '"'){
			isDefinition = check_definition();
			if(isDefinition == -1)
				return false;
		}
		
		
		syntax_6502 = token[0] == '$';
		syntax_PIC = (token[0] == 'H' && token[1] == '\'');
		syntax_Intel = (token[0] == '0' && token[1] == 'X');
		syntax_GAS = token[0] == '%';
		isDecimal = (token[0] >= 0x30 && token[0] <= 0x39) && token[1] != 'X';
		isHexadecimal = syntax_6502 || syntax_PIC || syntax_Intel;
		
		
		if(isHexadecimal || syntax_GAS || isDecimal || reg_index != -1){
			if(!isMnemonic){
		        printerr("Invalid mnemonic");
		        return false;
			}		
		    isMnemonic = false;
		    operand = token;
		}else{
			if(isMnemonic){
				if(isInclude){
					break;
				}
				printerr("Invalid operand");
				return false;
			}
			isMnemonic = true;
			mnemonic = token;
			
			
			toIgnore = strcmp(token, "DEFINE") == 0;
			
			if(toIgnore)
				return true;
				
			mnemonic_index = get_mnemonic();
			if(mnemonic_index == -1){
				token[strcspn(&token[0], ":")] = 0;
				return calc_label(token);
			}
		
			isOrg = mnemonic_index == 47;
			isInclude = mnemonic_index == 48;
			isAllocator = mnemonic_index == 43 || mnemonic_index == 44 || mnemonic_index == 45 || mnemonic_index == 46;
			if(isAllocator || isInclude){
				break;
			}
				
		}
		
        token = strtok(NULL, " ");
        count_tok++;
    }
    
	return true;	
}
// -----------------------------------------------------------------------------

// parser: it's the sintatic analyzer step checking the syntax
// -----------------------------------------------------------------------------
bool parser(){
	if(isAllocator){
		proc_dcb();
		return true;
	}
	if(isInclude){
		return true;
	}
		
	if(!isMnemonic){
		if(!addressing[mnemonic_index]){
			printerr("Instruction expect 0 operand. Given 1");
			return false;
		}
		
		if(!parse_addressing(((isHexadecimal) ? 2 : 1))) return false;
	}else{
		if(addressing[mnemonic_index]){
			printerr("Instruction expect 1 operand. Given 0");
			return false;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------

// parse_addressing: Called by parser(), interpret the numbers format syntax
// -----------------------------------------------------------------------------
bool parse_addressing(int index){
		char op[50] = {0};
		int operand_len = strlen(operand);
		
		bool isBitGetter = false;
		int count = 0;
		int op_int = (reg_index == -1) ? 0 : reg_index;
		if(reg_index == -1){
			int i = (operand[0] == '$' || isDecimal) ? index - 1 : index;
			
			for(; i < operand_len; i++){
				isBitGetter = (operand[i] == ':' && operand[i+1] == ':');
				if(operand[i] == ';' || operand[i] == '\'' || isBitGetter)	
					break;
				
				op[count++] = operand[i];
			}
			
			memcpy(dest, op, count+1);
			int base = (isHexadecimal) ? 16 : 10;
			number = strtol(dest, &endptr, base);
			if (*endptr != '\0') {
				printerr("Cannot parse the hexa or decimal number");
				return false;
			}
			
			len = strlen(dest);
			
			bit_shift = 0;
			if(isBitGetter){
				count = 0;
				i += 2;
				for(int j = 0; j < 10; j++) op[j] = 0;
				for(; i < operand_len; i++){
					if(operand[i] == ';') break;
					op[count++] = operand[i];
				}
				memcpy(dest, op, count+1);
				int bits = strtol(dest, &endptr, 10);
				number = (number & (0x00F << bits)) >> bits;
				
				curr_refer->bitshift = bits;
				curr_refer->isHigh = isBitGetter;
			}
			
			
			op_int = number;
		}else{
			number = op_int;
			len = 1;
		}
		
	
		bool is3bitreg = reg_index != -1 && !isLabel && !isRelative;
		bool is4bit = (op_int > 7 && op_int < 16) && !isLabel && !isRelative;
		bool is8bit = (op_int > 15 && op_int < 256) && !isLabel && !isRelative;
		bool is12bit = ((op_int > 255 && op_int < 4096) && isDecimal) || (isLabel || isRelative);
		bool isJump = (addressing[mnemonic_index] & REL) == REL;
		bool isImmediate = (addressing[mnemonic_index] & IMM) == IMM;
		bool isRegister = (addressing[mnemonic_index] & REG) == REG;
		isRelative = isJump;
		
		if((isImmediate || isJump || isOrg) && is3bitreg){
			printerr("Cannot use register addressing");
			return false;
		}
		if(isOrg && op_int > 4095){
			printerr("Exceeded the limit bound. Larger than 12-bit");
			return false;
		}
		if((mnemonic_index == 0x0A || mnemonic_index == 0xB) && is4bit){
			printerr("Exceeded the limit bound. Using 4-bit address");
			return false;
		}
		if(mnemonic_index >= 0 && mnemonic_index < 0xD && !isBitGetter){
			if(is12bit){
				printerr("Exceeded the limit bound. Using 12-bit address");
				return false;
			}else if(is8bit){
				printerr("Exceeded the limit bound. Using 8-bit address");
				return false;
			}	
		}
		if(isRegister && is4bit){
			printerr("Exceeded the limit bound. Using 4-bit address");
			return false;
		}
		
	return true;	
}
// -----------------------------------------------------------------------------

// generator: It's the machine code generation, can be the semantic analyzer too
// -----------------------------------------------------------------------------
bool generator(){
	if(dcb_process())	
		return true;
	if(isOrg){
		proc_org();
		return true;
	}
	if(isInclude){
		proc_include();
		return !directive_error;
	}
		
	
    unsigned char opcode = opcodes[mnemonic_index];
    char operand_byte1, operand_byte2;
    
    if(addressing[mnemonic_index] & REG || addressing[mnemonic_index] & IMM){
		operand_byte1 = (char) number & 0xFF;
    	opcode += operand_byte1;
    	code_address[code_index++] = opcode;
	}else{
		if(addressing[mnemonic_index] & REL && isRelative){
			if(number != 0xFFFF){
				operand_byte1 = (char) (((number - (code_index + 2)) & 0xF00) >> 8);
				operand_byte2 = (char) ((number - (code_index + 2)) & 0xFF);
						
				opcode |= operand_byte1;
						
				if(mnemonic_index == 42)
					if(operand_byte1 & 0x8)
						opcode += 0x20;
						
				code_address[code_index++] = opcode;
				code_address[code_index++] = operand_byte2;		
			}else{
				code_address[code_index++] = opcode;
				code_address[code_index++] = 0x00;	
			}
			
			 
		}else{
			code_address[code_index++] = opcode;
		}
	}
    
	return true;
}
// -----------------------------------------------------------------------------
// **********************************************************************************

// FUNCTIONS TO MANAGE AND GENERATE THE FILES
// **********************************************************************************

// changeExtension: switch the extension name from the source file by other extension
// -----------------------------------------------------------------------------
char* changeExtension(const char *filename, const char* ext){
	char *newName = malloc(strlen(filename) + 1);
	strcpy(newName, filename);
	
	char *point = strrchr(newName, '.');
	if(point != NULL){
		strcpy(point, ext);
	}else{
		strcat(newName, ext);
	}
	
	return newName;
}
// -----------------------------------------------------------------------------

// writeHex: create the hexadecimal file logisim-compatible
// -----------------------------------------------------------------------------
int writeHex(const char *filename, unsigned char *machinecode, size_t size){
	FILE *f = fopen(filename, "w");
	if(!f){
		perror("Error in opening the file!\n");
		exit(1);
	}
	
	const char* header = "v2.0 raw";
	fprintf(f, "%s\n", header);
	
	for(size_t i = 0; i < size; i++){
		fprintf(f, "%02X ", machinecode[i]);
		if((i + 1) % 16 == 0){
			fprintf(f, "\n");
		}
	}
	
	if(size % 16 != 0){
		fprintf(f, "\n");
	}
	
	fclose(f);
	return (size * 3) + ceil((double)size / 16) * 2 + strlen(header) + 2;
}
// -----------------------------------------------------------------------------

// writeBin: create the raw binary file for possible emulators
// -----------------------------------------------------------------------------
void writeBin(const char *filename, unsigned char *machinecode, size_t size){
	FILE *f = fopen(filename, "wb");
	if(!f){
		perror("Error in opening the file!\n");
		exit(1);
	}
	
	fwrite(machinecode, 1, size, f);
	
	fclose(f);
}
// -----------------------------------------------------------------------------

// load_file_to_buffer: Read the file and store in a memory buffer
// -----------------------------------------------------------------------------
char *load_file_to_buffer(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error in opening the file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize + 1);
    if (!buffer) {
        perror("Error in allocate memory");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    fclose(file);

    if (read_size != filesize) {
        fprintf(stderr, "Error: imcomplete reading of file\n");
        free(buffer);
        return NULL;
    }

    buffer[filesize] = '\0';

    return buffer;
}
// -----------------------------------------------------------------------------

// buffer_fgets: Modification of standard fgets for buffer reading
// -----------------------------------------------------------------------------
char *buffer_fgets(char *line, size_t maxlen, const char **bufptr) {
    if (**bufptr == '\0') return NULL;

    size_t i = 0;
    while (i < maxlen - 1 && **bufptr != '\0' && **bufptr != 0x0D) {
        line[i++] = *(*bufptr)++;
    }

    if (**bufptr == 0x0D) {
        line[i++] = *(*bufptr)++;
        if(**bufptr == 0x0A)
        	line[i++] = *(*bufptr)++;
    }

    line[i] = '\0';
    return line;
}
// -----------------------------------------------------------------------------
// **********************************************************************************

// FUNCTIONS TO PREPROCESS AND ASSEMBLE THE FILE OR BUFFER
// **********************************************************************************

// preprocess_file: Preprocessor for the file loading method
// Read the filename, preprocess and return a state of fail or success
// presenting preprocessing details in verbose state
// -----------------------------------------------------------------------------
bool preprocess_file(const char *filename, bool verbose){
	
	isVerbose = verbose;
	FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error in opening the file");
        return false;
    }
    
    linenum = 1;
    isBuffer = false;
    currentfile = filename;
    
    if(!listInitialized){
	    define_list = begin_def();
	    dcb_list = begin_dcb();
	    label_list = begin_lab();
		listInitialized = true;	
	}
    
    while (fgets(line, sizeof(line), file)){
    	int x = 0;
    	for(; line[x] == 0x20 || line[x] == 0x09; x++);
    	if(strcmp(&line[x], "\n") == 0){
    		linenum++;
    		continue;
		}
		
    	format_line();
    	if(verbose) printf("Preprocessor Line: %s\n", line);
    	
    	int i = 0;
    	for(; line[i] == ' '; i++);
    	if(line[i] == 0x0D){
			continue;	
		}
		if(line[i] == NULL){
			break;
		}
    	int length = strcspn(&line[i], ":");
		token = strtok(line, " ");
		
		if(token == NULL || token[0] == ';') {
			linenum++;
			continue;
		}
		
		while (token != NULL) {
	    	isLineComment = token[0] == ';' || isLineComment;
	    	if(isLineComment){
	    		token = strtok(NULL, " ");
	    		continue;
			}
			
			directive_error = false;
			isDirective = false;
			isMnemonic = false;
			if(get_directive() == -1){
				if(get_mnemonic() == -1){
					label = token;
					if(!get_label(length))
						return false;
				}
				break;	
			}else if(directive_error)
					return false;
					
			token = strtok(NULL, " ");
		}

		linenum++;
	}

	fclose(file);
	return true;
}
// -----------------------------------------------------------------------------

// assemble_file: Assembler for the file loading method
// read the filename, assemble and return a fail or success state.
// write the machine code buffer to compiled param.
// present assembler details from the verbose state
// -----------------------------------------------------------------------------
bool assemble_file(const char *filename, unsigned char **compiled, bool verbose) {
	isVerbose = verbose;
	bool isValid = false;
	if(!isInclude){
		isValid = preprocess_file(filename, verbose);
		if(!isValid) return false;	
	}

	if(memory == NULL){
		memory = (unsigned char *) malloc(MEMORY_EMULATOR * sizeof(unsigned char));
		if (memory == NULL) {
	        printf("Error in allocate memory.");
	        return 0;
	    }
    	code_address = memory;
	}
	
    linenum = 1;
    isBuffer = false;
    currentfile = filename;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error in opening the file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
    	if(verbose) printf("Assembly line: %s", line);
    	int x = 0;
    	for(; line[x] == 0x20 || line[x] == 0x09; x++);
    	if(strcmp(&line[x], "\n") == 0){
    		linenum++;
    		continue;
		}
		
        // Lexycal Analyze and tokenization
		isValid = tokenizer();
        if(!isValid)
        	break;
        if(toIgnore){
        	linenum++;
        	continue;
		} 
		
		// Sintatic Analyze
		isValid = parser();
		if(!isValid)
        	break;
        
		// Semantic Analyze and generation
		isValid = generator();
		if(!isValid)
        	break;
		
		// Free temporary allocation
		if(isDefinition) 
			free(token);
			
        linenum++;
    }

	if(code_index > 4096){
		perror("Error: The maximum program size is 4096 bytes.");
        exit(EXIT_FAILURE);
	}
	
		
    fclose(file);
	
	*compiled = code_address;
	if(verbose) printf("Chegou fora do while!\n");
	
	return isValid;
}
// -----------------------------------------------------------------------------

// preprocess_buffer: Preprocessor for the buffer loading method
// read the buffer and preprocess, presenting preprocessing details in verbose
// -----------------------------------------------------------------------------
bool preprocess_buffer(const char *buffer, bool verbose){
	isVerbose = verbose;
    const char *bufptr = buffer;
    int linenum = 1;
    
    if(!listInitialized){
	    define_list = begin_def();
	    dcb_list = begin_dcb();
	    label_list = begin_lab();
		listInitialized = true;	
	}
    
    while (buffer_fgets(line, sizeof(line), &bufptr)){
    	isBuffer = true;
    	if(line[0] == 0x0D && line[1] == 0x0A){
    		linenum++;
    		continue;
		}
		
    	format_line();
    	
    	if(verbose) printf("Preprocessor Line: %s\n", line);
    	
    	int i = 0;
    	for(; line[i] == ' '; i++);
    	if(line[i] == 0x0D){
			continue;	
		}
		if(line[i] == NULL){
			break;
		}
    	int length = strcspn(&line[i], ":");
		token = strtok(line, " ");
		
		if(token == NULL || token[0] == ';') {
			linenum++;
			continue;
		}
		
		while (token != NULL) {
	    	isLineComment = token[0] == ';' || isLineComment;
	    	if(isLineComment){
	    		token = strtok(NULL, " ");
	    		continue;
			}
			
			directive_error = false;
			isDirective = false;
			isMnemonic = false;
			if(get_directive() == -1){
				if(get_mnemonic() == -1){
					label = token;
					if(!get_label(length))
						return false;
				}
				break;	
			}else if(directive_error)
					return false;
					
			token = strtok(NULL, " ");
		}

		linenum++;
	}
	
	return true;
}
// -----------------------------------------------------------------------------

// assemble_buffer: Assembler for the buffer loading method
// read the buffer and assembler writing the machine code memory to compiled param
// presenting assembler detail from the verbose state.
// -----------------------------------------------------------------------------
bool assemble_buffer(const char *buffer, unsigned char **compiled, bool verbose) {
	isVerbose = verbose;
    bool isValid = false;
	if(!isInclude){
		isValid = preprocess_buffer(buffer, verbose);
		if(!isValid) return false;
	}

	if(memory == NULL){
		memory = (unsigned char *) malloc(MEMORY_EMULATOR * sizeof(unsigned char));
		if (memory == NULL) {
	        printf("Error in allocate memory");
	        return 0;
	    }
    	code_address = memory;
	}
	
	const char *bufptr = buffer;
    int linenum = 1;
    
    while (buffer_fgets(line, sizeof(line), &bufptr)) {
    	isBuffer = true;
    	if(verbose) printf("Assembly line: %s", line);
        if (line[0] == 0x0D && line[1] == 0x0A) {
            linenum++;
            continue;
        }

        // Lexycal Analyze and tokenization
		isValid = tokenizer();
        if(!isValid)
        	break;
        if(toIgnore){
        	linenum++;
        	continue;
		} 
		
		// Sintatic analyze
		isValid = parser();
		if(!isValid)
        	break;
        
		// Semantic analyze and generation
		isValid = generator();
		if(!isValid)
        	break;
		
		// Free temporary allocation
		if(isDefinition) 
			free(token);
        
        linenum++;
    }

    if(code_index > 4096){
		perror("Error: The maximum program size is 4096 bytes.");
        exit(EXIT_FAILURE);
	}
	
	*compiled = code_address;
	return isValid;
}
// -----------------------------------------------------------------------------
// **********************************************************************************

// FUNCTIONS TO CHECK CODE DETAILS
// **********************************************************************************

// hex_dump: Print hexadecimal bytes from machine code in a formatted way
// -----------------------------------------------------------------------------
void hex_dump(unsigned char* code){
	unsigned short address = 0x000;
	printf("\nCode Length: %d\n", code_index);
	for(int i = 0; i < code_index; i++){
		if(i % 16 == 0)
			printf("\n0x%03X:", address);

		printf(" %02X", code[i]);
		address++;
	}
}
// -----------------------------------------------------------------------------

// get_code_size: Get the machine code length
// -----------------------------------------------------------------------------
int get_code_size(){
	return code_index;
}
// -----------------------------------------------------------------------------
// **********************************************************************************

// FUNCTIONS TO CLOSE AND RESET DATA
// **********************************************************************************

// close_lists: Free the lists allocation
// -----------------------------------------------------------------------------
void close_lists(){
	if(define_list != NULL) 
		freedef(define_list);
	if(dcb_list != NULL);
		freedcb(dcb_list);
	if(label_list != NULL)
		freelab(label_list);
}
// -----------------------------------------------------------------------------

// reset_states: Turns all the states in false for reset
// -----------------------------------------------------------------------------
void reset_states(){
	isMnemonic = false;
	isLabel = false;
	isRelative = false;
	isLineComment = false;
	isAllocator = false;
	isOrg = false;
	isHigh = false;
	syntax_GAS = false;
	syntax_Intel = false;
	syntax_PIC = false;
	syntax_6502 = false;
	dcb_index = 0;
	number = 0;
}
// -----------------------------------------------------------------------------
// **********************************************************************************

#endif
