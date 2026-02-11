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

bool calc(const char*, int*, bool);

#include "wr80list.h"	// WR80 list Structures for labels, defines and DBs
#include "wr80data.h"	// WR80 Variables, Structs and Data for Assembler
#include "astlib.h"		// WR80 AST Library for math expression evaluations
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

bool create_label(char* label, int addr){
	DefineList* def = getdef(define_list, label);
	if(def == NULL){
		LabelList* lab = getLabelByName(label_list, label);
		if(lab == NULL){
			label_list = insertlab(label_list, linenum, label, addr);	// 0x0000
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
	return true;
}

bool calc(const char* formula, int* result, bool step){
	bool state = true;
	is_asm_proc = step;
	
	AST *tree = parse(formula);
	*result = eval(tree, &state);
	
	free_ast(tree);
	
	return state;
}

void free_operand(){
    if (operand) {
        free(operand);
        operand = NULL;
    }
}


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

void line_to_upper(){
	int linesize = strlen(line);
	for(int i = 0; i < linesize; i++){
		if(line[i] > 0x60 && line[i] < 0x7B)
			line[i] -= 0x20;
	}
}

int find(const char *str, const char *substr) {
    int len_str = strlen(str);
    int len_sub = strlen(substr);

    if (len_sub == 0 || len_sub > len_str)
        return -1;

    for (int i = 0; i <= len_str - len_sub; i++) {
        int j = 0;
        while (j < len_sub && str[i + j] == substr[j]) {
            j++;
        }
        if (j == len_sub)
            return i;
    }

    return -1;
}
// -----------------------------------------------------------------------------

void free_vector(char** vector, int size){
    if (vector == NULL) return;

    for (int i = 0; i < size; i++) {
        if (vector[i] != NULL) {
            free(vector[i]);
            vector[i] = NULL;
        }
    }
    free(vector);
    vector = NULL;
}

// FUNCTIONS TO PROCESS DIRECTIVES IN PREPROCESSOR
// **********************************************************************************

// proc_org: Organize and Allocate memory intervals filling with Zeros (alignment)
// -----------------------------------------------------------------------------
void proc_org(){
	if(alloc){
		org_num = 0;
		if(code_index <= number){
			for(; code_index < number; code_index++){
				code_address[code_index] = 0x00;
			}	
		}else{
			printwarn("Is not possible to organize this memory location");
			return;	
		}	
	}else{
		org_num = number;
	}
}
// -----------------------------------------------------------------------------

void proc_rep(){
	int linetmp1 = linenum + 1;	// 2
	
	//printf("isBuffer? %d\n", isBuffer);
	char* repcode = (isBuffer) 	? get_code_buffer(block[REP_I].begin, block[REP_I].end, &bufferget)
								: get_code(block[REP_I].begin, block[REP_I].end);
	
	linenum = linetmp1;
	unsigned char* code;
	int codesize = number;
	for(int i = 0; i < codesize; i++){
		int linetmp = linenum;
		linebegin = linetmp1;
		linesrc = linenum;
		const char* buffer = bufferget;
		//printf("repcode: %s\n", repcode);
		bool assembled = (repcode != NULL) 	? assemble_buffer(repcode, &code, false)
											: false;
		repstate = true;
		bufferget = buffer;
		
		linenum = linetmp;
		if(!assembled){
			directive_error = !assembled;
			return;
		}
	}
}

// proc_dcb: Allocate data byte or data word
// -----------------------------------------------------------------------------
/*
void proc_dcb(){
	token = strtok(NULL, " ");
	operand = token;
	
	int comm_index = strcspn(token, ";");
	token[comm_index] = '\0';
	operand[comm_index] = '\0';
	
	int i = 0;
	int length = 0;
	char value[1024] = {0};
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
				value[length++] = token[i++];
			}
			value[length] = '\0';
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
		
			int argstate = get_arg(name);
			if(argstate != -1)
				if(!argstate) return; else continue;
			
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
			
			while(token[i] != ',' && token[i] != '\0' && token[i] != ' ')
				val[j++] = token[i++];
			val[j] = 0;
			if(token[i-1] == '\'')
				val[--j] = 0;
			
			bool isDW = mnemonic_index == 53;
			int base = (isHexa) ? 16 : 10;
			int num = strtol(val, &endptr, base);
			if(((j > 2 && isHexa) || (num > 255 && !isHexa)) && !isBitIsolate && !isDW)
				printwarn("DCB byte is larger than 8-bit. Only low byte will be considered");
		
			//value = realloc(value, length+1);
			
			if(isDW){
				value[length++] = (num & 0xFF);
				value[length++] = (num & 0xFF00) >> 8;
			}else{
				value[length++] = (isHighByte) ? (num & 0xFF00) >> 8 : num & 0xFF;
			}
			value[length] = '\0';
			
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
	//free(value);
}
*/


void proc_dcb()
{
    token = strtok(NULL, "");
    if (!token) return;

    operand = token;

    /* Remove comentário */
    int comm = strcspn(token, ";");
    token[comm] = '\0';

    char value[1024] = {0};
    int length = 0;

    bool isDW = (mnemonic_index == 53);

    char *item = strtok(token, ",");

    while (item)
    {
        /* Trim espaços */
        while (*item == ' ' || *item == '\t') item++;

        char *end = item + strlen(item) - 1;
        while (end > item && (*end == ' ' || *end == '\t')) {
            *end-- = '\0';
        }

        if (*item == '\0') {
            item = strtok(NULL, ",");
            continue;
        }

        /* --------- Caso: STRING --------- */
        if (*item == '"')
        {
            item++; /* pula " */

            while (*item && *item != '"') {
                value[length++] = *item++;
            }

            item = strtok(NULL, ",");
            continue;
        }

        /* --------- Caso: Expressão --------- */

        bool isLowByte  = false;
        bool isHighByte = false;

        if (*item == '<') {
            isLowByte = true;
            item++;
        }
        else if (*item == '>') {
            isHighByte = true;
            item++;
        }

        /* Trim novamente após < ou > */
        while (*item == ' ' || *item == '\t') item++;

        /* get_arg (mantido conforme pedido) */
        int argstate = get_arg(item);
        if (argstate != -1) {
            if (!argstate) return;
            item = strtok(NULL, ",");
            continue;
        }
		
        /* Avaliação da expressão */
        int result = 0;
        if (!calc(item, &result, true)) {
            directive_error = true;
            printerr("PARSE => undefined value");
            return;
        }
		
        /* Warning para DB */
        if (!isDW && !isLowByte && !isHighByte) {
            if (result > 255 || result < -128)
                printwarn("DCB byte is larger than 8-bit. Only low byte will be considered");
        }

        /* Armazenamento */
        if (isDW)
        {
            value[length++] = result & 0xFF;
            value[length++] = (result >> 8) & 0xFF;
        }
        else
        {
            if (isHighByte)
                value[length++] = (result >> 8) & 0xFF;
            else
                value[length++] = result & 0xFF;
        }

        item = strtok(NULL, ",");
    }

	dcb_index = length;
    dcb_list = insertdcb(dcb_list, linenum, length, value);
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
		if(token != NULL && isMacroScope) 
			if(!get_arg(token)) return;
		
		switch(pos){
			case 1:	name = strdup(token);
					break;
			case 2:	value = strdup(token);
					break;
				
			default: {
				if(token != NULL){
					int comm_index = strcspn(token, ";");
					
					if(token[comm_index] != ';'){
						value = (char*) realloc(value, (strlen(value) + strlen(token) + 1) * sizeof(char));
						strcat(value, token);
					}else{
						if(comm_index == 0)
							token = NULL;
						else{
							token[comm_index] = '\0';
							value = (char*) realloc(value, (strlen(value) + strlen(token) + 1) * sizeof(char));
							strcat(value, token);
							token = NULL;
						}	
					}
				}
				
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
		}else{
			MacroList* macro = getMacroByName(macro_list, name);
			if(macro != NULL){
				if(!isBuffer)
					printf("%s -> Error: This name '%s' at line %d is already defined at line %d.", currentfile, macro->name, linenum, macro->line);
				else
					printf("Error: This name '%s' at line %d is already defined at line %d.", macro->name, linenum, macro->line);
				directive_error = true;
				return;
			}
		}
	}
		
	bool finish = false;
	
	if(value[0] != '#'){
		//if(value[0] == '$'){
		//	strtol(&value[1], &endptr, 16);
		//	if (*endptr != '\0') {
		//		printerr("Invalid defined value - hexa error");
		//		directive_error = true;
		//	}
		//}else{
			int number_res = 0;

			if(!calc(value, &number_res, false)) {
				define_list = insertdef(define_list, linenum, name, NULL, value);
				finish = true;
			}else{
				char result[10] = {0};
				sprintf(result, "%d", number_res);
				free(value);
				value = strdup(result);
			}
		//}
	}
	if(directive_error || finish){
		free(name);
		free(value);
		return;
	}
	define_list = insertdef(define_list, linenum, name, value, NULL);
	free(name);
	free(value);
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

	int result = get_arg(token);
	if(!result){
		return;
	}else if(result != -1){
		token = strtok(token, "\"");	
	}
	strncpy(file_name, token, strlen(token) + 1);

    long file_size = 0;
	int linetemp = linenum;
	char* filetemp = currentfile;
	bool mounted = false;
	if(!isInclude){
		if(isBuffer){
			linebegin = 1;
			char *source_code = load_file_to_buffer(file_name, &file_size);
			if(file_size)
			    mounted = preprocess_buffer(source_code, isVerbose);
			free(source_code);
		}else{
			//printf("preprocessing include...\n");
			mounted = preprocess_file(file_name, isVerbose);
			//printf("preprocessed? = %d\n", mounted);
		}
	}else{
		unsigned char* machinecode = NULL;
		if(isBuffer){
			linebegin = 1;
			char *source_code = load_file_to_buffer(file_name, &file_size);
			if(file_size)
			    mounted = assemble_buffer(source_code, &machinecode, isVerbose);
			free(source_code);
		}else{
			//printf("assembling include...\n");
			mounted = assemble_file(file_name, &machinecode, isVerbose);
			//printf("assembled? = %d\n", mounted);
		}
		isInclude = false;
		if(isVerbose) {
			hex_dump(machinecode);
			printf("\n");	
		}
	}
	linenum = linetemp;
	currentfile = filetemp;
	
	if (!mounted) {
		fprintf(stderr, "Error: error in assemble the included file: %s\n", file_name);
		directive_error = true;
	}
}
// -----------------------------------------------------------------------------

// proc_includeb: include binaries
// -----------------------------------------------------------------------------
void proc_includeb(){
	char file_name[128] = {0};
	long file_size = 0;
	token = strtok(NULL, "\"");

	if (token == NULL) {
		fprintf(stderr, "Error: empty file name in include directive.\n");
		directive_error = true;
		return;
	}

	int result = get_arg(token);
	if(!result){
		return;
	}else if(result != -1){
		token = strtok(token, "\"");	
	}
	
	strncpy(file_name, token, strlen(token) + 1);
	char *binary_data = load_file_to_buffer(file_name, &file_size);
	memcpy(&code_address[code_index], binary_data, file_size);
	code_index += file_size;
	free(binary_data);
}
// -----------------------------------------------------------------------------

// proc_export: Export a label function externally
// -----------------------------------------------------------------------------
void proc_export(){
	token = strtok(NULL, " ");
	
	if(token != NULL){
		int size_str = strlen(token) + 1;
		wll_table_alloc += size_str + 6;
		wll_str_pointer += 6;
		wll_counter++;
		label_pointer = (char**) realloc(label_pointer, wll_counter * sizeof(char*));
		label_pointer[wll_counter - 1] = strdup(token);
	}else{
		fprintf(stderr, "Error: empty label name for export\n");
		directive_error = true;
	}
}
// -----------------------------------------------------------------------------

// proc_import: Import a label function externally
// -----------------------------------------------------------------------------
void proc_import(){
	token = strtok(NULL, "\" ,\t\r\n");
	
	if (token == NULL) {
		fprintf(stderr, "Error: empty label name for import\n");
		directive_error = true;
		return;
	}
	
	long size = 0;
	int files_counter = 0;
	int symbols_counter = 0;
	int symbol_count = 0;
	char** imported_files = NULL;
	char** imported_symbols = NULL;
	char** symbol_as_label = NULL;
	char** file_data = NULL;
	int* symbols_found = NULL;
	bool is_as_command = false;
	int linetmp = linenum;
	
	while(token != NULL){
		imported_files = (char**) realloc(imported_files, ++files_counter * sizeof(char*));
		imported_files[files_counter - 1] = strdup(token);
		token = strtok(NULL, "\" ,\t\r\n");
		//printf("file: '%s'\n", imported_files[files_counter - 1]);
	}
	file_data = (char**) malloc(files_counter * sizeof(char*));
	for(int i = 0; i < files_counter; i++)
		file_data[i] = NULL;
		
	char* import_data = get_code(block[IMP_I].begin, block[IMP_I].end);
	linenum = linetmp;
	
	token = strtok(import_data, " ,\t\r\n");
	while(token != NULL){
		if(strcmp(token, "AS") == 0){
			token = strtok(NULL, " ,\t\r\n");
			is_as_command = true;
			continue;		
		}
		
		if (!is_as_command){
			imported_symbols = (char**) realloc(imported_symbols, ++symbols_counter * sizeof(char*));
			symbol_as_label = (char**) realloc(symbol_as_label, symbols_counter * sizeof(char*));
			imported_symbols[symbols_counter - 1] = strdup(token);
			symbol_as_label[symbols_counter - 1] = NULL;
			//printf("symbol: '%s'\n", imported_symbols[symbols_counter-1]);
		}else{
			symbol_as_label[symbols_counter - 1] = strdup(token);
			is_as_command = false;
		}
		
		token = strtok(NULL, " ,\t\r\n");
	}
	
	symbols_found = (int*) malloc(symbols_counter * sizeof(int));
	for(int i = 0; i < symbols_counter; i++)
		symbols_found[i] = 0;
		
	for(int j = 0; j < symbols_counter; j++){
		bool next_file = false;
		for(int i = 0; i < files_counter; ){
			bool func_found = false;
			
			if(!file_data[i])
				file_data[i] = load_file_to_buffer(imported_files[i], &size);
				
			if(file_data[i][0] == 'W' && file_data[i][1] == 'L' && file_data[i][2] == 'L'){
				int funcs_count = file_data[i][3];
				for(int w = 0; w < funcs_count; w++){
					int index = 4 + 6 * w;
					int str_addr = ((file_data[i][index + 1] & 0xFF) << 8) | (file_data[i][index] & 0xFF);
					if(strcmp(&file_data[i][str_addr], imported_symbols[j]) == 0){
						
						LabelList* list = getLabelByName(label_list, imported_symbols[j]);
						if(list != NULL && !next_file){
							for(int x = 0; x < symbols_counter; x++){
								if(strcmp(imported_symbols[j], imported_symbols[x]) == 0){
									symbols_found[x] = (symbols_found[x] + 1) % files_counter;
									i = symbols_found[x];
									next_file = true;
									break;
								}
							}
						}else{
							next_file = false;
						}
						
						if(next_file) break;
						func_found = true;
						
						if(!create_label((!symbol_as_label[j]) ? imported_symbols[j] : symbol_as_label[j], code_index)){
							directive_error = true;
							return;
						}
						int code_addr = ((file_data[i][index + 3] & 0xFF) << 8) | (file_data[i][index + 2] & 0xFF);
						int code_size = ((file_data[i][index + 5] & 0xFF) << 8) | (file_data[i][index + 4] & 0xFF);
						memcpy(&code_address[code_index], &file_data[i][code_addr], code_size);
						code_index += code_size;
						break;
					} // if function found
					
				} // funcs end
				
				if(next_file)
					continue;
				
			}else{
				printf("Error: Invalid WLL File - No Signature.");
				directive_error = true;
				return;
			} // if signature valid
			
			if(func_found){
				break;
			}else{
				if(i == files_counter - 1){
					printf("Error: Symbol '%s' not found in '%s' file", imported_symbols[j], imported_files[i]);
					directive_error = true;
					return;
				}
				i++;
			}
			
		} // files end
	} // symbols end
	
	//showlab(label_list);
	for(int i = 0; i < files_counter; i++){
		free(file_data[i]);
		free(imported_files[i]);
	}
	free(file_data);
	free(imported_files);
	
	for(int i = 0; i < symbols_counter; i++){
		free(imported_symbols[i]);
		free(symbol_as_label[i]);
	}
	free(imported_symbols);
	free(symbol_as_label);
	free(symbols_found);
	free(import_data);
	
	linenum += 2;
}
// -----------------------------------------------------------------------------

// proc_endx: Finish export command
// -----------------------------------------------------------------------------
void proc_endx(){
	int code_size = (code_index + org_num) - wll_code_start;
	code_address[4 + 6 * wll_index + 4] = (code_size & 0xFF);
	code_address[4 + 6 * wll_index + 5] = (code_size & 0xFF00) >> 8;
	wll_str_pointer += strlen(label_pointer[wll_index]) + 1;
	wll_index++;
}
// -----------------------------------------------------------------------------

// proc_macro: Store Macros in lists for replacement
// -----------------------------------------------------------------------------
void proc_macro(){
	int pos = 1;
	char* name = NULL;
	char* params = NULL;
	char** pnames = NULL;
	char* code = NULL;
	int linen = linenum;
	int argc = 0;
	
	while(token != NULL){
		//int comm_index = strcspn(token, ";");
		//token[comm_index] = '\0';
		
		switch(pos){
			case 1:{
				token = strtok(NULL, " ");
				if(token == NULL) break;
				int len = strlen(token);
				name = malloc(len + 1);
				strcpy(name, token);
				name[len] = '\0';
				break;
			}	
			case 2: {
				argc = strtol(token, &endptr, 10);
				if (*endptr != '\0') {
					if(strcmp(token, "...") != 0){
						argc = 0;
						pnames = parse_parameters(&argc); // LEAK: Fluxo	
					}else{
						argc = -1;
					}
				}
				token = NULL;
				continue;
				break;
			}
			default: {
				if(token != NULL && token[0] != ';'){
					printerr("Invalid defined token");
					directive_error = true;
				}
				token = NULL;
				continue;
			}
		}
		
		token = strtok(NULL, ",");
		pos++;
	}
	
	bool isNum = name[0] > 0x30 && name[0] <= 0x39;
	bool isSymbol = false;
	for(int i = 0; i < strlen(name); i++)
		isSymbol = name[i] == '#' || name[i] == '$' || name[i] == ':' || name[i] == '%' || isSymbol;
	if(params != NULL){
		for(int i = 0; i < strlen(params); i++)
			isSymbol = params[i] == '#' || params[i] == '$' || params[i] == ':' || params[i] == '%' || isSymbol;	
		free(params);
		params = NULL;
	}
		
	if(isNum || isSymbol){
		printerr("Invalid declared macro");
		directive_error = true;
		return;
	}
	
	MacroList* macro = getMacroByNameA(macro_list, name, argc);
	if(macro != NULL){
		if(!isBuffer)
			printf("%s -> Error: This name '%s' at line %d is already defined at line %d.", currentfile, macro->name, linenum, macro->line);
		else
			printf("Error: This name '%s' at line %d is already defined at line %d.", macro->name, linenum, macro->line);
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
		}else{
			DefineList* def = getdef(define_list, name);
			if(def != NULL){
				if(!isBuffer)
					printf("%s -> Error: This name '%s' at line %d is already defined at line %d.", currentfile, def->name, linenum, def->line);
				else
					printf("Error: This name '%s' at line %d is already defined at line %d.", def->name, linenum, def->line);
				directive_error = true;
				return;
			}
		}
	}

	code = get_code(block[MACRO_I].begin, block[MACRO_I].end);
	macro_list = insertmac(macro_list, argc, name, pnames, code, linen);	// LEAK: Fluxo
	if(name != NULL) free(name);
	if(code != NULL) free(code);
}
// -----------------------------------------------------------------------------

void skip_if(int cmd_i){
	if(!isBuffer){
		skip_block(block[cmd_i].begin, block[cmd_i].end);
	}else{
		skip_block_buffer(block[cmd_i].begin, block[cmd_i].end, &bufferget);
	}
	hasif = true;
	ifstate = false;
}

void assemble_if(int cmd_i){
	int linetmp1 = linenum + 1;
	char* ifcode = (isBuffer) 	? get_code_buffer(block[cmd_i].begin, block[cmd_i].end, &bufferget)
								: get_code(block[cmd_i].begin, block[cmd_i].end);
	
	//if(isMacroScope) printf("Assemblando IF... code -> %s\n", ifcode); //debug
	linenum = linetmp1;
	int linetmp = linenum;
	linebegin = linetmp1;
	linesrc = linenum;
	unsigned char* code;
	const char* buffer = bufferget;

	bool assembled = (ifcode != NULL) 	? assemble_buffer(ifcode, &code, isVerbose)
										: false;
	ifstate = true;
	hasif = true;
	bufferget = buffer;
	linenum = linetmp;
	//printf("assembled: %d\n", assembled); //debug
	directive_error = !assembled;
	free(ifcode);
	ifcode = NULL;
}

bool getIfValue(char** name1) {
	token = *name1;
	int i = (token[0] == '!') ? 1 : 0;
			
	get_arg(&token[i]);

	i = (token[0] == '!') ? 1 : 0;
	
	int param = -1;
	if (isMacroScope)
		param = getArgIndex(&token[i]);
		
	DefineList* defines = getdef(define_list, (char*)&token[i]);
	LabelList* labels = (defines == NULL) ? getLabelByName(label_list, (char*)&token[i]) : NULL;
	MacroList* macros = (labels == NULL) ? getMacroByName(macro_list, (char*)&token[i]) : NULL;
	
	char* new_value = NULL;
	if (defines != NULL) {
		new_value = (defines->refs[0] == 0) ? strdup(defines->value) : strdup(defines->refs);
	}
	else if (macros != NULL) {
		new_value = strdup(macros->name);
	}
	else if (labels != NULL) {
		new_value = strdup(itoa(labels->addr, endptr, 10));
	}
	else if (param != -1) {
		new_value = strdup(currmacro->pvalues[param]);
	}
	else {
		new_value = strdup(&token[i]);
	}
	
	// Libera o valor antigo e substitui por new_value
	if (*name1 != NULL)
		free(*name1);
	
	token = new_value;
	get_arg(token);
	*name1 = strdup(token);
	
	//printf("MACRO : %s, VALUE: %s\n", currmacro->name, *name1);

	// Condição booleana
	bool nameCondition = (!i)
		? (defines != NULL || labels != NULL || macros != NULL || param != -1)
		: (defines == NULL && labels == NULL && macros == NULL && param == -1);

	return nameCondition;
}

char* tokentmp;
void check_if(bool condition, int cmd_i){
	if(condition){
		assemble_if(cmd_i);
	}else{
		token = tokentmp;
		skip_if(cmd_i);
	}
}

bool check_number(const char* name, int* num){
	*num = strtol(name, &endptr, 10);
	if(*endptr != '\0') return false;
	return true;
}

// -----------------------------------------------------------------------------
// IF function
void proc_if(){
	int pos = 1;
	char* name1 = NULL;
	char* op = NULL;
	char* name2 = NULL;
	tokentmp = token;
	
	if(ifstate){
		if(isELSE){
			skip_if(ELSE_I);
			elsestate = false;
			ifstate = false;
			return;
		}else{
			ifstate = false;	
		}
	}else{
		if(isELSE){
			assemble_if(ELSE_I);
			elsestate = true;
			ifstate = false;
			return;
		}
	}
	
	ifdepth++;
	while((token = strtok(NULL, " ")) != NULL){
		switch(pos){
			case 1: name1 = strdup(token);
					break;
			case 2: op = strdup(token);
					break;
			case 3: name2 = strdup(token);
					break;
		}
		pos++;
	}
//	if(isMacroScope) printf("name1: %s, op: %s, name2: %s\n", name1, op, name2); //debug
	if(name1 != NULL && op != NULL && name2 != NULL){
		
		bool hasName1 = getIfValue(&name1);
		bool hasName2 = getIfValue(&name2);
		token = tokentmp;
		
		int num1 = 0, num2 = 0;
		bool both_num = check_number(name1, &num1) && check_number(name2, &num2);
		
		if(strcmp(name1, "#$") == 0){
			bool isNaN = (strcmp(op, "==") == 0) ? !check_number(name2, &num2) : check_number(name2, &num2);
			check_if(isNaN, IF_I);
		}else if(strcmp(name2, "#$") == 0){
			bool isNaN = (strcmp(op, "==") == 0) ? !check_number(name1, &num1) : check_number(name1, &num1);
			check_if(isNaN, IF_I);
		}else if(strcmp(op, "==") == 0){
			check_if(strcmp(name1, name2) == 0, IF_I);
		}else if(strcmp(op, "!=") == 0){
			check_if(strcmp(name1, name2) != 0, IF_I);
		}else if(strcmp(op, ">=") == 0){
			check_if(both_num && (num1 >= num2), IF_I);
		}else if(strcmp(op, "<=") == 0){
			check_if(both_num && (num1 <= num2), IF_I);
		}else if(strcmp(op, ">") == 0){
			check_if(both_num && (num1 > num2), IF_I);
		}else if(strcmp(op, "<") == 0){
			check_if(both_num && (num1 < num2), IF_I);
		}else if(strcmp(op, "%") == 0){
			check_if(both_num && !(num1 % num2), IF_I);
		}else if(strcmp(op, "&") == 0){
			check_if(both_num && (num1 & num2), IF_I);
		}else if(strcmp(op, "&&") == 0){
			check_if((both_num && (num1 && num2)) || (hasName1 && hasName2), IF_I);
		}else if(strcmp(op, "|") == 0){
			check_if(both_num && (num1 | num2), IF_I);
		}else if(strcmp(op, "||") == 0){
			check_if((both_num && (num1 || num2)) || (hasName1 || hasName2), IF_I);
		}else if(strcmp(op, "^") == 0){
			check_if(both_num && (num1 ^ num2), IF_I);
		}
		
		
		free(name1);
		free(op);
		free(name2);
	}else{
		if(op == NULL && name1 != NULL){
			check_if(getIfValue(&name1), IF_I);
			free(name1);
		}else{
			printerr("Invalid IF operation syntax");
			return;
		}
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
bool recursive_def(char **value, int *num) {
    char *str = *value;
    char *endptr;

    int base = (str[0] != '$' &&
               (str[0] != '0' || str[1] != 'X') &&
               (str[0] != 'H' || str[1] != '\'')) ? 10 : 16;

    int index = (base == 16 && str[0] == '$') ? 1 : 0;

    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\'')
        str[len - 1] = '\0';

    *num = strtol(&str[index], &endptr, base);
	
    if (*endptr == '\0')
        return true;

	LabelList* label = NULL;
    DefineList* definition = getdef(define_list, str);
    if (!definition){
    	label = getLabelByName(label_list, str);
    	if(!label)
    		return false;
	}
		
    free(*value);

    if(definition){
    	if (definition->refs[0] == 0) {
	        *value = strdup(definition->value);
	    } else {
	        *value = strdup(definition->refs);
	        return false;
	    }
	}else{
		char buffer[32] = {0};
		if(label->addr == 0xFFFF){
			bool isRel = (addressing[mnemonic_index] & REL) == REL;
            bool isIMM = isAllocator;
            bool isDW  = mnemonic_index == 53;
            int addr_index = code_index + dcb_index;
            label->refs = insertaddr(label->refs, addr_index, isRel, isIMM, isHigh, isDW);
            curr_refer = label->refs;
            curr_refer->isExpression = true;
            sprintf(buffer, "%d", 0);
        }else{
        	sprintf(buffer, "%d", label->addr);
        	curr_refer = NULL;
		}
        
		*value = strdup(buffer);
	}

    return recursive_def(value, num);
}


// -----------------------------------------------------------------------------

char* replace(const char* token, const char* old_substr, const char* new_substr) {
    static char* buffer = NULL;
    static size_t buffer_capacity = 0;

    // Se token aponta para dentro do buffer estático, precisamos copiá-lo
    // para um local temporário antes de potencialmente free() o buffer.
    char *local_copy = NULL;
    const char *src = token;
    if (buffer != NULL && token >= buffer && token < buffer + buffer_capacity) {
        local_copy = strdup(token);   // cria cópia independente do token
        if (!local_copy) return (char*)token; // falha em alocar: fallback
        src = local_copy;
    }

    char* pos = strstr(src, old_substr);
    if (!pos) {
        if (local_copy) free(local_copy);
        return (char*)token;
    }

    int old_len = (int)strlen(old_substr);
    int new_len = (int)strlen(new_substr);
    int original_len = (int)strlen(src);

    int new_size = original_len + (new_len - old_len) + 1; // +1 para '\0'

    // Realocar APENAS se o buffer for pequeno
    if (buffer_capacity < new_size) {
        if (buffer) {
            free(buffer); // agora seguro: src já é cópia se apontava para buffer
            buffer = NULL;
            buffer_capacity = 0;
        }
        buffer = (char*)malloc(new_size);
        if (!buffer) {
            // cleanup da cópia temporária se existia
            if (local_copy) { free(local_copy); local_copy = NULL; }
            buffer_capacity = 0;
            return (char*)token;
        }
        buffer_capacity = new_size;
    }

    int prefix_len = (int)(pos - src); // pos aponta dentro de src (não em token, se copiamos)

    memcpy(buffer, src, prefix_len);
    memcpy(buffer + prefix_len, new_substr, new_len);
    strcpy(buffer + prefix_len + new_len, src + prefix_len + old_len);
    buffer[new_size - 1] = '\0';

    // liberamos a cópia temporária porque já copiamos tudo para buffer
    if (local_copy) {
        free(local_copy);
        local_copy = NULL;
    }

    return buffer;
}


// -----------------------------------------------------------------------------

// replace_name: replace de defined name to the value
// -----------------------------------------------------------------------------
int replace_name(char* name){
    char *value = NULL;
    char value_buf[32];
    DefineList* definition = getdef(define_list, name);
    
    if(definition == NULL){
        LabelList* label = getLabelByName(label_list, name);
        
        if(label != NULL){
            bool isRel = (addressing[mnemonic_index] & REL) == REL;
            bool isIMM = isAllocator;
            bool isDW  = mnemonic_index == 53;

            if(isRel)
                isRelative = true;
            else
                isLabel = true;
                
            if(label->addr == 0xFFFF){
                int addr_index = code_index + dcb_index;
                label->refs = insertaddr(label->refs, addr_index, isRel, isIMM, isHigh, isDW);
            }
            
            curr_refer = label->refs;

            sprintf(value_buf, "%d", label->addr);
            value = value_buf;
        }else{
        	int number_res = 0;
        	
            if(!calc(name, &number_res, true)){
                printerr("LABEL NULL => undefined value");
                return -1;
            }
            
            sprintf(value_buf, "%d", (unsigned short)number_res & 0xFFF);
            value = value_buf;

            token = replace(token, name, value);
            strtol(token, &endptr, 10);
            return (*endptr != '\0') ? replace_name(value) : 1;	
        }
    }else{
        if (definition->refs[0] == 0){
            value = definition->value;
        }else{
            int number_res = 0;

            if(!calc(definition->refs, &number_res, true)){
                printerr("DEFINITION NOT-NULL => undefined value");
                return -1;
            }

            sprintf(value_buf, "%d", number_res);
            value = value_buf;

            token = replace(token, name, value);

            strtol(token, &endptr, 10);
            return (*endptr != '\0') ? replace_name(value) : 1;
        }
    }

    token = replace(token, name, value);
    return 1;
}

// -----------------------------------------------------------------------------

int getParamIndex(const char* param){
	if(currmacro->pnames == NULL) return -1;
	for(int i = 0; i < currmacro->pcount; i++)
		if(strcmp(currmacro->pnames[i], param) == 0)
			return i;
	return -1;
}

int getArgIndex(const char* arg){
	if(currmacro->pvalues == NULL) return -1;
	int size = (currmacro->pcount == -1) ? currmacro->argsc : currmacro->pcount;
	for(int i = 0; i < size; i++)
		if(strcmp(currmacro->pvalues[i], arg) == 0)
			return i;
	return -1;
}

int check_register(bool is_gas_syntax){
	for(int i = 0; i < 16; i++){
		if(strcmp(user_registers[i], token) == 0 || strcmp(port_registers[i], token) == 0){
			reg_index = (is_gas_syntax) ? i - 8 : i;
			return reg_index;
		}
	}
	return -1;
}

char* check_symbol(const char* name){
	static char argument[32] = {0};
	memset(argument, 0, 32);
	int param = 0;
	switch(name[1]){
		case '*': 	param = (currmacro->pcount != -1) ? currmacro->pcount : currmacro->argsc;
					snprintf(argument, sizeof(argument), "%d", param);
					break;
		case '+':	param = (currmacro->pcount != -1) ? currmacro->pcount + 1 : currmacro->argsc + 1;
					snprintf(argument, sizeof(argument), "%d", param);
					break;
		case '-':	param = (currmacro->pcount != -1) ? currmacro->pcount - 1 : currmacro->argsc - 1;
					snprintf(argument, sizeof(argument), "%d", param);
					break;
		case '.': {
			int size = (currmacro->pcount != -1) ? currmacro->pcount : currmacro->argsc;
			currmacro->indexp = (currmacro->indexp >= size) ? 0 : currmacro->indexp;
			snprintf(argument, sizeof(argument), "%s", currmacro->pvalues[currmacro->indexp++]);
			break;
		}
		case '#': {
			snprintf(argument, sizeof(argument), "%d", ((isMacroScope) ? ++currmacro->ilabelB : ++ilabelB));
			break;
		}
		case '$': {
			snprintf(argument, sizeof(argument), "%s", name);
			break;
		}
		case '%': {
			int size = (currmacro->pcount != -1) ? currmacro->pcount : currmacro->argsc;
			currmacro->indexp = (currmacro->indexp >= size) ? 0 : currmacro->indexp;
			snprintf(argument, sizeof(argument), "%s", currmacro->pvalues[currmacro->indexp]);
			break;
		}
	}
	
	return argument;
}

int get_named_arg(const char* name){
	char* argument = check_symbol(name);
	if(argument[0] == 0){
		int param = getParamIndex(&name[1]);
		if(param == -1){
			printf("%s -> Error at line %d: Param '%s' does not exist!\n", currentfile, linenum, &name[1]);
			return 0;
		}
		snprintf(argument, sizeof(argument), "%s", currmacro->pvalues[param]);
	}
	
	token = replace(token, name, argument);
	return 1;	
}

int get_enum_arg(const char* name, int arg){
	if(arg < 1) arg = 1;
	int argc = (currmacro->pcount != -1) ? currmacro->pcount : currmacro->argsc;
	if(arg > argc){
		printf("%s -> Error at line %d: arg #%d is out of limit bound specified by line %d!\n", currentfile, linenum, arg, linesrc);
		return 0;
	}
	
	token = replace(token, name, currmacro->pvalues[arg-1]);	// LEAK: Fluxo
	//printf("MACRO: %s, MNEMONIC: %s, ARGUMENT: %s\n", currmacro->name, mnemonic, currmacro->pvalues[arg-1]);
	return 1;
}

int get_arg(const char* name){
	if(name[0] == '#'){
		int arg = strtol(&name[1], &endptr, 10);
		directive_error = (*endptr != '\0') ? !get_named_arg(name) : !get_enum_arg(name, arg);
		if(directive_error) return 0;
		return 1;
	}
	return -1;
}


char* get_code(const char* beg_cmd, const char* end_cmd) {
    char* code = NULL;
    int total_size = 0;
    int depth = 1; // já estamos dentro do bloco principal
    linenum++;
    linebegin = linenum;
    char* line_tmp = NULL;
    
    while (fgets(line, sizeof(line), fileopened)) {
		linenum++;	// 5
		line_to_upper();
		//printf("line macro: %s\n", line);
        // copia original da linha
        char line_tmp[sizeof(line)];
        strcpy(line_tmp, line);
		//if(line_tmp != NULL) free(line_tmp);
		//line_tmp = strdup(line);

        // tokeniza para detectar comandos
        //token = strtok(line, "\n\t ;");
        token = strtok(line, "\n");
		token = strtok(token, " ");
		token = strtok(token, "\t");
		token = strtok(token, ";");
        if (!token) continue;

        if (strcmp(token, beg_cmd) == 0) {
            depth++; // achamos rep aninhado
        } else if (strcmp(token, end_cmd) == 0) {
            depth--; // achamos endp ou endm
            if (depth == 0) {
                break; // fim do bloco principal
            }
        }

        // armazena a linha no buffer
        int len = strlen(line_tmp);
        int new_size = total_size + len + 1 + 1;
        code = realloc(code, new_size);
        if (total_size == 0) code[0] = '\0';
        if (line_tmp[len - 1] == '\n') {
		    line_tmp[len - 1] = '\r'; // troca \n por \r
			line_tmp[len] = '\n';     // coloca \n depois
		    line_tmp[len + 1] = '\0'; // encerra string
		}
        strcat(code, line_tmp);
        //total_size += len;
        total_size = strlen(code);
    }
    return code;
}

char* get_code_buffer(const char* beg_cmd, const char* end_cmd, const char** buffer) {
    char* code = NULL;
    int total_size = 0;
    int depth = 1; // já estamos dentro do bloco principal
    linenum++;
    linebegin = linenum;
    char* line_tmp = NULL;

    while (buffer_fgets(line, sizeof(line), buffer)) {
		linenum++;
		line_to_upper();
		
        // copia original da linha
        char line_tmp[sizeof(line)];
        strcpy(line_tmp, line);
		//if(line_tmp != NULL) free(line_tmp);
		//line_tmp = strdup(line);

        // tokeniza para detectar comandos
        //token = strtok(line, "\n\t ;");
        token = strtok(line, "\n");
		token = strtok(token, " ");
		token = strtok(token, "\t");
		token = strtok(token, ";");
		token[strlen(end_cmd)] = '\0';
        if (!token) continue;

        if (strcmp(token, beg_cmd) == 0) {
            depth++; // achamos rep aninhado
        } else if (strcmp(token, end_cmd) == 0) {
            depth--; // achamos endp ou endm
            if (depth == 0) {
                break; // fim do bloco principal
            }
        }

        // armazena a linha no buffer
        int len = strlen(line_tmp);
        int new_size = total_size + len + 1 + 1;
        code = realloc(code, new_size);
        if (total_size == 0) code[0] = '\0';
        if (line_tmp[len - 1] == '\n') {
		    line_tmp[len - 1] = '\r'; // troca \n por \r
			line_tmp[len] = '\n';     // coloca \n depois
		    line_tmp[len + 1] = '\0'; // encerra string
		}
        strcat(code, line_tmp);
        //total_size += len;
        total_size = strlen(code);
    }
    return code;
}

bool skip_block(const char* begin, const char* end) {
    if (strcmp(token, begin) == 0) {
        int depth = 1; // já estamos dentro de um rep
        while (fgets(line, sizeof(line), fileopened)) {
        	line_to_upper();
            linenum++;		// 6

            //token = strtok(line, "\n\t ;");
            token = strtok(line, "\n");
			token = strtok(token, " ");
			token = strtok(token, "\t");
			token = strtok(token, ";");
            if (!token) continue;

            if (strcmp(token, begin) == 0) {
                depth++; // achamos outro rep
            } else if (strcmp(token, end) == 0) {
                depth--; // achamos um endp
                if (depth == 0) {
                    break; // este é o endp que fecha o bloco inicial
                }
            }
        }
        
        return true;
    }
    return false;
}

bool skip_block_buffer(const char* begin, const char* end, const char** buffer) {
    if (strcmp(token, begin) == 0) {
        int depth = 1; // já estamos dentro de um rep
        while (buffer_fgets(line, sizeof(line), buffer)) {
        	line_to_upper();
            linenum++;

            //token = strtok(line, "\n\t ;");
            token = strtok(line, "\n");
			token = strtok(token, " ");
			token = strtok(token, "\t");
			//token = strtok(token, ";");
			token[strlen(end)] = '\0';
            //if (!token) continue;

            if (strcmp(token, begin) == 0) {
                depth++; // achamos outro rep
            } else if (strcmp(token, end) == 0) {
                depth--; // achamos um endp
                if (depth == 0) {
                    break; // este é o endp que fecha o bloco inicial
                }
            }
        }
        //*buffer = bufptr;
        return true;
    }
    return false;
}

// check_definition: Verify if the operand has defined name and replace it
// -----------------------------------------------------------------------------
int check_definition(){
	int index = 0;
	bool isMacroArg = token[index] == '#';
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

	if(!isMacroArg)
		if(name[0] >= 0x30 && name[0] <= 0x39)
			return 0;
		
	name[namelen] = 0;
	int i = (isMacroArg) ? 1 : 0;
	int arg = strtol(&name[i], &endptr, 10);
	
	if(*endptr != '\0'){
		if(!isMacroArg){
			strtol(&name[0], &endptr, 16);
			bool possibleHexaError = (!index && (token[index] != '$' 
									&& (token[index] != '0' && token[index+1] != 'X') 
									&& (token[index] != 'H' && token[index+1] != '\'')));
			if(*endptr != '\0' || possibleHexaError){
				if(replace_name(name) != -1){
					return check_definition();
				}else{
					directive_error = true;
					return -1;
				}
			}	
		}else{
			if(!get_named_arg(name)) return -1;
			int reg_index = check_register(token[0] == '%');
			if(reg_index == -1) check_definition();
			return 1;
		}
	}else if(isMacroArg){
		if(!get_enum_arg(name, arg)) return -1;
		int reg_index = check_register(token[0] == '%');
		if(reg_index == -1) check_definition();
		return 1;
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
			func_ptr();	// LEAK: Fluxo (proc_macro)
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
	//if(isMacroScope){
	int pos = find(label, "##");
	if(pos != -1){
		char argument[32] = {0};
		memset(argument, 0, 32);
		int ilabA = (isMacroScope) ? ++currmacro->ilabelA : ++ilabelA;
		snprintf(argument, sizeof(argument), "%d", ilabA);
		label = replace(label, "##", argument);
	}
	//}
		
	MacroList* macro = getMacroByName(macro_list, label);
	while(token != NULL && macro == NULL){		
		
		int pos = strcspn(&label[0], ":");
		if(label[pos] == ':'){
			token = strtok(NULL, " ");
			if(token != NULL || label[pos+1] != '\0'){
				printerr("Invalid label name - incorrect char");
				return false;
			}
						
			label[pos] = '\0';
			strtol(label, &endptr, 10);
			
			if(*endptr == '\0' || (label[0] >= 0x30 && label[0] <= 0x39)){
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
			
			if(!create_label(label, 0xFFFF))
				return false;
										
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
				//printf("label: %s\n", label);
				printerr("Invalid label name - missing ':'");
				return false;
			}	
		}	
	}
	return true;
}
// -----------------------------------------------------------------------------

char** parse_parameters(int *argc_out) {
    char **pvalues = NULL;
    *argc_out = 0; // zera o contador que será retornado
	char* tmp = NULL;
    // Continua a partir de onde strtok parou (após _mov)
    //char *token = token;

    while (token != NULL) {
        // Remove espaços no início
        while (*token == ' ' || *token == '\t') token++;
		
		token[strcspn(token, " ")] = '\0';
		
		DefineList *defines = getdef(define_list, token);
		if(defines != NULL){
			if(tmp != NULL) free(tmp);
			tmp = (defines->refs[0] == 0) ? strdup(defines->value) : strdup(defines->refs);
			token = tmp;
		}
		
		int result = get_arg(token);
		//if(!result)
		//	return NULL;
			
		int pos = find(token, "##");
		if(pos != -1){
			char argument[32] = {0};
			memset(argument, 0, 32);
			int ilabB = (isMacroScope) ? ++currmacro->ilabelB : ++ilabelB;
			snprintf(argument, sizeof(argument), "%d", ilabB);
			token = replace(token, "##", argument);
		}
			
        // Remove espaços e \n no final
        size_t len = strlen(token);
        while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n' || token[len - 1] == '\r'))
            token[--len] = '\0';

        // Realoca mais espaço no vetor de ponteiros
        char **temp = realloc(pvalues, (*argc_out + 1) * sizeof(char*));	// LEAK: Raiz
        if (temp == NULL) {
            // Libera caso dê erro
            for (int i = 0; i < *argc_out; i++) free(pvalues[i]);
            free(pvalues);
            return NULL;
        }
        pvalues = temp;

        // Copia a string do token
        pvalues[*argc_out] = malloc(len + 1);
        if (pvalues[*argc_out] == NULL) {
            for (int i = 0; i < *argc_out; i++) free(pvalues[i]);
            free(pvalues);
            return NULL;
        }
        //strcpy(pvalues[*argc_out], token);
        memcpy(pvalues[*argc_out], token, len + 1); // copia incluindo '\0'
        (*argc_out)++;

        // Próximo token separado por vírgula
        token = strtok(NULL, ",");
    }

    return pvalues;
}



// calc_label: calculate the label address on assembler
// -----------------------------------------------------------------------------
bool calc_label(unsigned char *label){
	LabelList* list = getLabelByName(label_list, label);
	
	if(list != NULL){
		list->addr = code_index + org_num;
		
		if(isExport){
			if(strcmp(label_pointer[wll_index], label) == 0){
				code_address[4 + 6 * wll_index] = (wll_str_pointer & 0xFF);
				code_address[4 + 6 * wll_index + 1] = (wll_str_pointer & 0xFF00) >> 8;
				code_address[4 + 6 * wll_index + 2] = (list->addr & 0xFF);
				code_address[4 + 6 * wll_index + 3] = (list->addr & 0xFF00) >> 8;
				wll_code_start = list->addr;
				strcpy(&code_address[wll_str_pointer], label_pointer[wll_index]);
			}
		}
		
		if(list->refs != NULL){
			setref(list->refs, code_address, list->addr, org_num);	
			freeref(list->refs);
			list->refs = NULL;
		}
		
		//showlab(label_list);
		toIgnore = true;
		return toIgnore;
	}else{
		MacroList* macro = getMacroByName(macro_list, label);
		if(macro == NULL){
			printerr("Unknown mnemonic");
			return false;
		}else{
			//if(isMacroScope) printf("macro: %s\n", label); // debug
			int argc = 0;
			token = strtok(NULL, ",");
			
			//if(isMacroScope) printf("param: %s\n", token); // debug
		    char **pvalues = parse_parameters(&argc); // LEAK: Fluxo

			invoked_macro = insertargs(macro_list, macro->name, argc, pvalues); // LEAK: Fluxo
		    
			if(invoked_macro != NULL){
				isMacro = true;
				isMacroScope = isMacro;
				return true;
			}else{
				printf("%s -> Error at line %d: Macro %s with %d args not found!\n", currentfile, linenum, macro->name, argc);
				return false;
			}
		}
	}
}
// -----------------------------------------------------------------------------


bool assemble_macro(){
	
	int linetmp = linenum;
	linesrc = linenum;
	MacroList *macrotmp = currmacro;
	currmacro = invoked_macro;
	linebegin = currmacro->line + 1;
	bool ifstate_tmp = ifstate;
	bool isELSE_tmp = isELSE;
	bool isIF_tmp = isIF;
	
	const char* buffertmp = bufferget;
	unsigned char* machinecode;
	bool assembled = assemble_buffer(currmacro->content, &machinecode, isVerbose);	// debug
	bufferget = buffertmp;
	
	isIF = isIF_tmp;
	isELSE = isELSE_tmp;
	ifstate = ifstate_tmp;
	currmacro = macrotmp;
	linenum = linetmp;
	
	return assembled;
}

// **********************************************************************************

// FUNCTIONS TO RUN EACH STEP OF THE ASSEMBLER
// **********************************************************************************

bool get_operand_states(){
	isMnemonic = false;
	syntax_6502 = token[0] == '$';
	syntax_PIC = (token[0] == 'H' && token[1] == '\'');
	syntax_Intel = (token[0] == '0' && token[1] == 'X');
	syntax_GAS = token[0] == '%';
	isDecimal = (token[0] >= 0x30 && token[0] <= 0x39) && token[1] != 'X' || token[0] == '-';
	isHexadecimal = syntax_6502 || syntax_PIC || syntax_Intel;
	
	reg_index = check_register(syntax_GAS);
	
	bool operand_state = isHexadecimal || syntax_GAS || isDecimal || reg_index != -1;
	
	return operand_state;
}

bool get_mnemonic_states(){
	isOrg = mnemonic_index == 54;
	isInclude = mnemonic_index == 55;
	isRepeat = mnemonic_index == 56;
	isIF = mnemonic_index == 57;
	isELSE = mnemonic_index == 58;
	isIncB = mnemonic_index == 59;
	isExportCurr = mnemonic_index == 60;
	isEndx = mnemonic_index == 61;
	isImport = mnemonic_index == 62;
	isExport = (!isEndx) ? isExportCurr || isExport : false;
	isAllocator = mnemonic_index == 50 || mnemonic_index == 51 || mnemonic_index == 52 || mnemonic_index == 53;
	
	bool mnemonic_state = isAllocator || isInclude || isIncB || isIF || isELSE || isExportCurr || isImport;
	
	return mnemonic_state;
}

char *saveptr;

void initial_spaced_token(){
	token = strtok(line, " ");
}

void next_spaced_token(){
	token = strtok(NULL, " ");
}

bool skip_attribs_line(){
	int linelen = strlen(line);
	for(int x = 0; x < linelen; x++){
        if(line[x] == ' ') 
			continue;
        toIgnore = (line[x] == 0x0D) || (line[x] == 0x0A);
        break;
    }
    initial_spaced_token();
    toIgnore = (token == NULL || token[0] == ';') || toIgnore;
    return toIgnore;
}

bool skip_line_comment(){
    if(!token) return true;
    isLineComment = (token[0] == ';') || isLineComment;
    return isLineComment;
}

void set_local_labels(){
	int pos = find(token, "##");
    if(pos != -1){
        char argument[32] = {0};
        int ilabB = 0;

        if(!isMnemonic)
            ilabB = (isMacroScope) ? ++currmacro->ilabelB : ++ilabelB;
        else
            ilabB = (isMacroScope) ? ++currmacro->ilabelC : ++ilabelC;

        snprintf(argument, sizeof(argument), "%d", ilabB);
        token = replace(token, "##", argument);
    }
}

void check_GAS_register(){
	syntax_GAS = token[0] == '%';
    reg_index = check_register(syntax_GAS);
}

bool skip_directives_command(){
	toIgnore = strcmp(token, "DEFINE") == 0 || toIgnore;
    toIgnore = skip_block(block[MACRO_I].begin, block[MACRO_I].end) || toIgnore;
    return toIgnore;
}

bool check_mnemonic(){
	isMnemonic = true;
    mnemonic = token;
	mnemonic_index = get_mnemonic();
    return (mnemonic_index != -1);
}

// format_operand: This function concat tokens in operands
bool get_operand(){
	next_spaced_token();
	if(!token)
		return false;
	
	operand = strdup(token);
		
	while(token){
		if(skip_line_comment()){
        	next_spaced_token();
        	continue;
    	}
		next_spaced_token();
		if(token){
			int new_size = strlen(operand) + strlen(token) + 1;
			char *tmp = realloc(operand, new_size);
	        if (!tmp) {
	            printf("error in realloc!\n");
	            return false;
	        }
	        operand = tmp;
	
	        strcat(operand, token);
		}
	}
	
	token = operand;
	int comm_index = strcspn(token, ";");
	token[comm_index] = '\0';
	
	return true;
}

// tokenizer: it's the lexycal analyzer step getting each token
// -----------------------------------------------------------------------------
bool tokenizer()
{
	int count_tok = 0;
	
    format_line();
    if(skip_attribs_line())
		return true;
		
    reset_states();
    set_local_labels();
    
	if(get_operand_states()){
		printerr("Invalid mnemonic");
        return false;
	}
    
    if(skip_directives_command())
    	return true;
    if(!check_mnemonic()){
    	token[strcspn(token, ":")] = 0;
        return calc_label(token);
	}
	
	if(get_mnemonic_states())
		return true;
    if(!get_operand())
    	return true;
    	
    get_operand_states();
    
    check_GAS_register();
    set_local_labels();
		
    get_operand_states();

    if(reg_index == -1 && token[0] != '"')
        if(check_definition() == -1)
			return false;
		
    operand = strdup(token);
    
    get_operand_states();
    
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
	if(isInclude || isIF || isELSE || isMacro || isIncB || isExportCurr || isEndx || isImport){
		return true;
	}
	
	if(!isMnemonic){
		if(!addressing[mnemonic_index]){
			printerr("Instruction expect 0 operand. Given 1");
			return false;
		}
		
		if(!parse_addressing(((isHexadecimal) ? 2 : 1))) return false; // UNADDRESSABLE ACCESS: Fluxo
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
		
		int operand_len = strlen(operand);	// UNADDRESSABLE ACCESS: Raiz
		
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
			
			int result = 0;
			
	        if(!calc(dest, &result, true)){
	            printerr("PARSE => undefined value");
	        	return false;
	        }
	
	        sprintf(dest, "%d", result);
			
			number = strtol(dest, &endptr, base);
			
			if(curr_refer)
				if(curr_refer->isExpression)
					number = 0xFFFF;
					
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
				int isolsize = 0x00F;
				if(mnemonic_index == 46)
					isolsize = 0xFF;
					
				number = (number & (isolsize << bits)) >> bits;
				
				if(curr_refer != NULL){
					curr_refer->bitshift = bits;
				 	curr_refer->isHigh = isBitGetter;
				 	curr_refer->is8bit = (isolsize == 0xFF);
				}
			}
			
			//printf("token: %s, number: %d\n", mnemonic, number);
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
		bool isImmediate = (addressing[mnemonic_index] & IMM) == IMM || 
							(addressing[mnemonic_index] & IMM2) == IMM2;
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
				if(mnemonic_index == 0x6){
					mnemonic_index = 46;
					return true;
				}
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

int calls = 0;
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
	if(isIncB){
		proc_includeb();
		return !directive_error;
	}
	
	if(isExportCurr) return true;
	if(isEndx){
		proc_endx();
		return !directive_error;
	}
	if(isImport){
		proc_import();
		return !directive_error;
	}
	
	if(isMacro){
		isMacro = !isMacro;
		
		calls++;
		bool isAssembled = assemble_macro(); // LEAK: Fluxo
		calls--;
		
		macroret = true;
		return isAssembled;	// isAssembled
	}
	if(isRepeat){
		proc_rep();
		
		return !directive_error;
	}
	if(isIF || isELSE){
		proc_if();
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
				int PC = code_index + org_num;
				operand_byte1 = (char) (((number - (PC + 2)) & 0xF00) >> 8);
				operand_byte2 = (char) ((number - (PC + 2)) & 0xFF);
						
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
			if(addressing[mnemonic_index] & IMM2){
				code_address[code_index++] = (char)(number & 0xFF);	
			}
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
char *load_file_to_buffer(const char *filename, long *filesize) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error in opening the file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(*filesize + 1);
    if (!buffer) {
        perror("Error in allocate memory");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, *filesize, file);
    fclose(file);

    if (read_size != *filesize) {
        fprintf(stderr, "Error: imcomplete reading of file\n");
        free(buffer);
        return NULL;
    }

    buffer[*filesize] = '\0';

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
bool preprocess_file(char *filename, bool verbose){
	
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
	    macro_list = begin_mac();
		listInitialized = true;	
	}
    
    while (fgets(line, sizeof(line), file)){
    	fileopened = file;
    	int x = 0;
    	if(line[x] == '\0'){
			break;
		}
    	for(; line[x] == 0x20 || line[x] == 0x09; x++);
    	if(strcmp(&line[x], "\n") == 0){
    		linenum++;
    		continue;
		}
		
		format_line();
		
    	if(verbose) printf("Preprocessor Line: %s\n", line);
    	
    	int i = 0;
    	int length = strcspn(&line[i], ":");
		token = strtok(line, " ");
		
		if(token == NULL || token[0] == ';') {
			linenum++;
			continue;
		}
		if(skip_block(block[REP_I].begin, block[REP_I].end)){
			linenum++;
			continue;
		}
		if(skip_block(block[IF_I].begin, block[IF_I].end)){
			linenum++;
			continue;
		}
		if(skip_block(block[ELSE_I].begin, block[ELSE_I].end)){
			linenum++;
			continue;
		}
		if(skip_block(block[IMP_I].begin, block[IMP_I].end)){
			linenum++;
			continue;
		}
		bool isAlloc = strcmp(token, "DB") == 0 || strcmp(token, "DW") == 0 || strcmp(token, "DCB") == 0 || strcmp(token, ".BYTE") == 0;
		
		bool isIncludeB = strcmp(token, "INCLUDEB") == 0 || strcmp(token, "ENDX") == 0;
		if(isAlloc || isIncludeB){
			linenum++;
			continue;	
		}
		
		
	    isLineComment = token[0] == ';' || isLineComment;
	    if(isLineComment){
	    	linenum++;
	    	continue;
		}
			
		directive_error = false;
		isDirective = false;
		isMnemonic = false;
		
		if(get_directive() == -1){	// LEAK: Fluxo
			if(get_mnemonic() == -1){
				label = token;
				if(!get_label(length))
					return false;
			}	
		}else{
			if(directive_error)
				return false;
		} 
			
		token = NULL;
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
bool assemble_file(char *filename, unsigned char **compiled, bool verbose) {
	isVerbose = verbose;
	bool isValid = false;
	if(!isInclude){
		isValid = preprocess_file(filename, verbose);	// lEAK: Fluxo
		if(!isValid) return false;	
	}
	//showdef(define_list);
	
	if(memory == NULL){
		memory = (unsigned char *) malloc(MEMORY_EMULATOR * sizeof(unsigned char));
		if (memory == NULL) {
	        printf("Error in allocate memory.");
	        return 0;
	    }
    	code_address = memory;
    	if(wll_table_alloc > 4){
    		code_index += wll_table_alloc;
    	    memset(&code_address[0], 0, wll_table_alloc);
    	    code_address[0] = 'W'; code_address[1] = 'L'; code_address[2] = 'L';
    	    code_address[3] = (char) wll_counter;
    	    wll_table_alloc = 4;
			if(verbose){
	    	    printf("\nExport Symbol Table: \n");
	    	    for(int i = 0; i < wll_counter; i++)
	    	    	printf("Symbol %d : '%s'\n", i, label_pointer[i]);				
			}
		}
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
    	fileopened = file;
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
        if(toIgnore){	linenum++;	continue;	} 
		
		// Sintatic Analyze
		isValid = parser();
		if(!isValid)
        	break;
        if(toIgnore){	linenum++;	continue;	}
        
		// Semantic Analyze and generation
		isValid = generator();
		if(!isValid)
        	break;
		
		if(hasif || macroret || repstate){
			file = fileopened;
			ifstate = false;
		}
		if(hasif) hasif = false;
		if(repstate) repstate = false;
		if(macroret) macroret = false;
		if(isMacroScope) isMacroScope = false;
	
		//free_operand();
        linenum++;
    }
	
	
	
	if(code_index > 4096){
		perror("Error: The maximum program size is 4096 bytes.");
        exit(EXIT_FAILURE);
	}
	
    fclose(file);
	
	*compiled = code_address;
	
	return isValid;
}
// -----------------------------------------------------------------------------

// preprocess_buffer: Preprocessor for the buffer loading method
// read the buffer and preprocess, presenting preprocessing details in verbose
// -----------------------------------------------------------------------------
bool preprocess_buffer(const char *buffer, bool verbose){
	isVerbose = verbose;
    const char *bufptr = buffer;
    linenum = linebegin;
    
    if(!listInitialized){
	    define_list = begin_def();
	    dcb_list = begin_dcb();
	    label_list = begin_lab();
	    macro_list = begin_mac();
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
		if(line[i] == '\0'){
			break;
		}
		
    	int length = strcspn(&line[i], ":");
		token = strtok(line, " ");
			
		if(token == NULL || token[0] == ';') {
			linenum++;
			continue;
		}
		if(skip_block_buffer(block[REP_I].begin, block[REP_I].end, &bufptr)){
			linenum++;
			continue;
		}
		if(skip_block_buffer(block[IF_I].begin, block[IF_I].end, &bufptr)){
			linenum++;
			continue;
		}
		if(skip_block_buffer(block[ELSE_I].begin, block[ELSE_I].end, &bufptr)){
			linenum++;
			continue;
		}
		if(skip_block(block[IMP_I].begin, block[IMP_I].end)){
			linenum++;
			continue;
		}
		
		bool isAlloc = strcmp(token, "DB") == 0 || strcmp(token, "DW") == 0 || strcmp(token, "DCB") == 0 || strcmp(token, ".BYTE") == 0;
		
		bool isExp = strcmp(token, "EXPORT") == 0;
		if(isExp){
			token = strtok(NULL, " ");
			if(token != NULL){
				wll_table_alloc += strlen(token) + 1 + 6;
			    wll_counter++;	
			}else{
				directive_error = true;
				return !directive_error;	
			}
		}
		bool isIncludeB = strcmp(token, "INCLUDEB") == 0 || isExp || strcmp(token, "ENDX") == 0;
		if(isAlloc || isIncludeB){
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
	
	isBuffer = false;
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
    	if(wll_table_alloc > 4){
    	    code_index += wll_table_alloc;
    	    memset(&code_address[0], 0, wll_table_alloc);
    	    code_address[0] = 'W'; code_address[1] = 'L'; code_address[2] = 'L';
    	    code_address[3] = (char) wll_counter;
    	    wll_table_alloc = 4;
			if(verbose){
	    	    printf("\nExport Symbol Table: \n");
	    	    for(int i = 0; i < wll_counter; i++)
	    	    	printf("Symbol %d : '%s'\n", i, label_pointer[i]);				
			}
		}
	}
	
	const char *bufptr = buffer;
	//const char *buffertmp = bufferget;
    linenum = linebegin;
    
    while (buffer_fgets(line, sizeof(line), &bufptr)) {
    	bufferget = bufptr;
    	const char* buftmp = bufptr;
    	
    	isBuffer = true;
    	if(verbose) printf("Assembly line Buffer: %s", line);
        if (line[0] == 0x0D && line[1] == 0x0A) {
            linenum++;
            continue;
        }

        // Lexycal Analyze and tokenization
		isValid = tokenizer();
        if(!isValid)
        	break;
        if(toIgnore){	linenum++;	continue;	} 
		
		// Sintatic analyze
		isValid = parser();
		if(!isValid)
        	break;
        if(toIgnore){	linenum++;	continue;	} 
        
		// Semantic analyze and generation
		isValid = generator();
		if(!isValid)
        	break;
		
		if(repstate || hasif || macroret){
			bufptr = bufferget;
		}
		if(repstate) repstate = false;
		if(hasif) hasif = false;
		if(macroret) {
			macroret = false;
			isMacro = false;
		}
        
        linenum++;
    }

	//bufferget = buffertmp;
	
    if(code_index > 4096){
		perror("Error: The maximum program size is 4096 bytes.");
        exit(EXIT_FAILURE);
	}

	isBuffer = false;
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
	if(macro_list != NULL){
		free_macrolist(macro_list);
	}
	if(label_pointer != NULL){
		for(int i = 0; i < wll_counter; i++)
			free(label_pointer[i]);
		free(label_pointer);
	}
}
// -----------------------------------------------------------------------------

// reset_states: Turns all the states in false for reset
// -----------------------------------------------------------------------------
void reset_states(){
	reg_index = -1;
	isDirective = false;
	isMnemonic = false;
	isLabel = false;
	isRelative = false;
	isLineComment = false;
	isAllocator = false;
	isInclude = false;
	isIncB = false;
	isImport = false;
	isExportCurr = false;
	isOrg = false;
	isHigh = false;
	syntax_GAS = false;
	syntax_Intel = false;
	syntax_PIC = false;
	syntax_6502 = false;
	dcb_index = 0;
	number = 0;
	toIgnore = false;
	
	//free_operand();
}
// -----------------------------------------------------------------------------
// **********************************************************************************

#endif
