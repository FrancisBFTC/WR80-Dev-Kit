/*
	WR80 List & Structs Library
	Created by Wender Francis (KiddieOS.Community)
	Date: 20/08/2025

*/

#ifndef __WR80LIST_H__
#define __WR80LIST_H__

// 1st list node for defines
struct node_def {
	int line;
	char name[256];
	char value[256];
	char refs[256];
	struct node_def * next;
};
typedef struct node_def DefineList;

// 2nd list node for DCBs, DBs
struct node_dcb {
	int line;
	int length;
	unsigned char* value;
	struct node_dcb * next;
};
typedef struct node_dcb DcbList;

// 3th list node label references
struct node_refs {
	int addr;
	bool relative;
	bool isDcb;
	bool isHigh;
	bool is8bit;
	bool isDW;
	int bitshift;
	struct node_refs * next;
};
typedef struct node_refs RefsAddr;

// 4th list node for labels
struct node_lab {
	int line;
	int addr;
	char name[256];
	RefsAddr * refs;
	struct node_lab * next;
};
typedef struct node_lab LabelList;

struct node_mac {
	int line;
	int pcount;
	int argsc;
	char id[256];
	char name[256];
	char** pnames;
	char** pvalues;
	char* content;
	int indexp;
	int ilabelA;
	int ilabelB;
	struct node_mac * next;
};
typedef struct node_mac MacroList;

MacroList* getMacroByName(MacroList*, char[]);
MacroList* getMacroByNameA(MacroList*, char[], int);
// DAT/TAD: Data Abstract Type Begin
// -----------------------------------------------------------------
// Initialize the define list
DefineList* begin_def(){
	return NULL;
}

// Initialize the dcb list
DcbList* begin_dcb(){
	return NULL;
}

// Initialize the label list
LabelList* begin_lab(){
	return NULL;
}

MacroList* begin_mac(){
	return NULL;
}

// Insert a new node in define list
DefineList* insertdef(DefineList* list, int line, char name[], char value[], char refs[]){
	DefineList *new_node = (DefineList*) malloc(sizeof(DefineList));
	strcpy(new_node->name, name);
	if(refs == NULL){
		strcpy(new_node->value, value);
		new_node->refs[0] = '\0';
	}else{
		strcpy(new_node->refs, refs);
		new_node->value[0] = '\0';
	}
	
	new_node->line = line;
	new_node->next = list;
	return new_node;
}

// Insert a new node in dcb list
DcbList* insertdcb(DcbList* list, int line, int length, char* value){
	DcbList *new_node = (DcbList*) malloc(sizeof(DcbList));
	new_node->line = line;
	new_node->length = length;
	new_node->value = (unsigned char*) malloc(length);
	memcpy(new_node->value, value, length);
	new_node->next = list;
	return new_node;
}

// Insert a new node in label list
LabelList* insertlab(LabelList* list, int line, char name[], int addr){
	LabelList *new_node = (LabelList*) malloc(sizeof(LabelList));
	strcpy(new_node->name, name);
	new_node->line = line;
	new_node->addr = addr;
	new_node->next = list;
	return new_node;
}

// Insert a new node in references list
RefsAddr* insertaddr(RefsAddr* list, int addr, bool relative, bool isdcb, bool isHigh, bool isDW){
	RefsAddr *new_node = (RefsAddr*) malloc(sizeof(RefsAddr));
	new_node->addr = addr;
	new_node->relative = relative;
	new_node->isDcb = isdcb;
	new_node->isHigh = isHigh;
	new_node->is8bit = false;
	new_node->isDW = isDW;
	new_node->next = list;
	return new_node;
}

MacroList* insertmac(MacroList* list, int argc, char name[], char** params, char* code, int line){
    MacroList *new_node = malloc(sizeof(MacroList));
    if (!new_node) {
        // se quiser, também liberar params aqui; mas caller espera insertmac cuide de params
        return NULL;
    }

    snprintf(new_node->id, sizeof(new_node->id), "%s_%d", name, argc);
    strncpy(new_node->name, name, sizeof(new_node->name) - 1);
    new_node->name[sizeof(new_node->name)-1] = '\0';
    new_node->pcount = argc;
    new_node->argsc = 0;
    new_node->line = line;
    new_node->indexp = 0;
    new_node->ilabelA = 0;
    new_node->ilabelB = 0;

    // pvalues inicia vazio (será preenchido em insertargs)
    new_node->pvalues = NULL;
    if (argc > 0) {
        // alocado como array de ponteiros inicializados a NULL
        new_node->pvalues = calloc(argc, sizeof(char*));
        if (!new_node->pvalues) {
            free(new_node);
            return NULL;
        }
    }

    // copia os nomes dos parâmetros (pnames) e liberta os params auxiliares
    if (argc > 0 && params != NULL) {
        new_node->pnames = calloc(argc, sizeof(char*));
        if (!new_node->pnames) {
            free(new_node->pvalues);
            free(new_node);
            return NULL;
        }
        for (int i = 0; i < argc; ++i) {
            new_node->pnames[i] = malloc(strlen(params[i]) + 1);
			if (new_node->pnames[i])
			    strcpy(new_node->pnames[i], params[i]);
            // libera o auxiliar
            free(params[i]);
            params[i] = NULL;
        }
        free(params);
        params = NULL;
    } else {
        new_node->pnames = NULL;
    }

    // content
    if (code != NULL) {
        new_node->content = malloc(strlen(code) + 1);
		if (new_node->content)
		    strcpy(new_node->content, code);
    } else {
        new_node->content = NULL;
    }

    new_node->next = list;
    return new_node;
}

MacroList* insertargs(MacroList *list, char name[], int argc, char** args){
    MacroList* macro = getMacroByNameA(list, name, -1);
	if(!macro){
		macro = getMacroByNameA(list, name, argc);
		if (!macro || !args) {
			if(macro) macro->argsc = 0;
        	return macro;
    	}	
	}else{
		macro->argsc = argc;
	}

    int param_count = macro->pcount;
    macro->indexp = 0;

    // garante que pvalues exista e tenha espaço
    if (!macro->pvalues && argc > 0) {
        macro->pvalues = calloc(argc, sizeof(char*));
        if (!macro->pvalues) {
            // tentamos liberar args para não vazar (para manter ownership consistente)
            for (int i = 0; i < argc; ++i) free(args[i]);
            free(args);
            return macro;
        }
    }

    // Copia cada argumento (libera qualquer conteúdo anterior)
    for (int i = 0; i < argc; ++i) {
        // se macro->pvalues[i] já tinha algo, libera para evitar leak
        if (macro->pvalues[i]) {
            free(macro->pvalues[i]);
            macro->pvalues[i] = NULL;
        }
        // copia do args — se args tiver menos elementos que pcount, trata NULL
        if (i < argc && args[i] != NULL) {
            macro->pvalues[i] = malloc(strlen(args[i]) + 1);
			if (macro->pvalues[i])
			    strcpy(macro->pvalues[i], args[i]);
        } else {
            macro->pvalues[i] = NULL;
        }
    }

    // libera array auxiliar args (ownership consumida)
    for (int i = 0; i < argc; ++i) {
        free(args[i]);
        args[i] = NULL;
    }
    free(args);
    args = NULL;

    return macro;
}


void free_macrolist(MacroList *list) {
    MacroList *cur = list;
    while (cur) {
        MacroList *next = cur->next;

        if (cur->pnames) {
            for (int i = 0; i < cur->pcount; ++i) {
                free(cur->pnames[i]);
            }
            free(cur->pnames);
        }
        if (cur->pvalues) {
            for (int i = 0; i < cur->pcount; ++i) {
                free(cur->pvalues[i]);
            }
            free(cur->pvalues);
        }
        if (cur->content) free(cur->content);
        // Se tiver campos adicionais, libere aqui...
        free(cur);

        cur = next;
    }
}

// search a definition by name
DefineList* search(DefineList *list, char* name){
	for(DefineList *li = list; li != NULL; li = li->next)
		if(strcmp(li->name, name) == 0)
			return li;
			
	return NULL;
}

// get a definition by name
DefineList* getdef(DefineList *list, char* name){
	for(DefineList *li = list; li != NULL; li = li->next)
		if(strcmp(li->name, name) == 0)
			return li;
			
	return NULL;
}

// get a allocate value by line
DcbList* getdcb(DcbList *list, int line){
	for(DcbList *li = list; li != NULL; li = li->next)
		if(li->line == line)
			return li;
			
	return NULL;
}

// get a label by line
LabelList* getLabelByLine(LabelList *list, int line){
	for(LabelList *li = list; li != NULL; li = li->next)
		if(li->line == line)
			return li;
			
	return NULL;
}

// get a label by name
LabelList* getLabelByName(LabelList *list, char name[]){
	for(LabelList *li = list; li != NULL; li = li->next)
		if(strcmp(li->name, name) == 0)
			return li;
			
	return NULL;
}

// get a Macro by name
MacroList* getMacroByName(MacroList *list, char name[]){
	for(MacroList *li = list; li != NULL; li = li->next)
		if(strcmp(li->name, name) == 0)
			return li;
			
	return NULL;
}

// get a Macro by name and argc
MacroList* getMacroByNameA(MacroList *list, char name[], int argc){
	for(MacroList *li = list; li != NULL; li = li->next)
		if(strcmp(li->name, name) == 0 && li->pcount == argc)
			return li;
			
	return NULL;
}

// calculate the referenced label before
void setref(RefsAddr *list, char *code_addr, int addr, int org_num){
	for(RefsAddr *li = list; li != NULL; li = li->next){
		int op_index = li->addr;
		if(li->relative){
			int PC = op_index + org_num;
			int rel_addr_high = (addr - (PC + 2) & 0xF00) >> 8;
			int rel_addr_low = (addr - (PC + 2)) & 0xFF;
			code_addr[op_index] |= rel_addr_high;
			code_addr[op_index+1] = rel_addr_low;
		}else{
			if(li->isDcb){
				code_addr[op_index] = (li->isHigh) ? (addr & 0xF00) >> 8 : (addr & 0xFF);
				if(li->isDW)
					code_addr[op_index + 1] = (addr & 0xF00) >> 8;
			}else{
				if(li->isHigh){
					int bits = li->bitshift;
					if(li->is8bit){
						int isolc = ((12 - bits) > 8) ? 8 : (12 - bits);
						unsigned char isol = (1 << isolc) - 1;
						code_addr[op_index+1] = (char)((addr & (isol << bits)) >> bits);
					}else{
						code_addr[op_index] = (char)((code_addr[op_index] & 0xF0) + ((addr & (0xF << bits)) >> bits));	
					}
				}else{
					code_addr[op_index] = (addr & 0xFF);
					code_addr[op_index+1] = (addr & 0xF00) >> 8;
				}
			}
		}
	}
}

// show each node the define list
void showdef(DefineList *list){
	for(DefineList *li = list; li != NULL; li = li->next){
		if(li->refs[0] == 0)
			printf("name = %s, value = %s, refs = none\n", li->name, li->value);
		else if(li->value[0] == 0)
				printf("name = %s, value = none, refs = %s\n", li->name, li->refs);
	}
		
		
}

// show each node the dcb list
void showdcb(DcbList *list){
	for(DcbList *li = list; li != NULL; li = li->next){
		printf("line = %d, length = %d,", li->line, li->length);
		printf(" values = ");
		for(int i = 0; i < li->length; i++)
			printf("%d,", li->value[i]);
		printf("\n");
	}
}

// show each node the label list
void showlab(LabelList *list){
	for(LabelList *li = list; li != NULL; li = li->next)
		printf("name = %s, addr = 0x%X, line = %d\n", li->name, li->addr, li->line);
}

// show each node the reference list
void showrefs(RefsAddr *list){
	for(RefsAddr *li = list; li != NULL; li = li->next)
		printf("addr = 0x%X, isHigh = %d, shift = %d\n", li->addr, li->isHigh, li->bitshift);
}

// show each node the label list
void showmac(MacroList *list){
	for(MacroList *li = list; li != NULL; li = li->next){
		printf(" line = %d\n ID = %s\n name = %s\n pcount = %d\n", li->line, li->id, li->name, li->pcount);
		if(li->pnames != NULL)
			for(size_t i = 0; i < li->pcount; i++)
				printf(" pnames[%zu] = '%s'\n", i, li->pnames[i]);
		if(li->pvalues != NULL)
			for(size_t i = 0; i < li->pcount; i++)
				printf(" pvalues[%zu] = '%s'\n", i, li->pvalues[i]);
		if(li->content != NULL)
			printf("%s", li->content);
		printf("\n");
	}
}

// free the define list
void freedef(DefineList *list){
	DefineList *aux = list;
	
	while(aux != NULL){
		DefineList *next_node = aux->next;
		free(aux);
		aux = next_node;
	}
}

// free the dcb list
void freedcb(DcbList *list){
	DcbList *aux = list;
	
	while(aux != NULL){
		DcbList *next_node = aux->next;
		free(aux->value);
		free(aux);
		aux = next_node;
	}
}

// free the define list
void freelab(LabelList *list){
	LabelList *aux = list;
	
	while(aux != NULL){
		LabelList *next_node = aux->next;
		free(aux);
		aux = next_node;
	}
}

// free the reference list
void freeref(RefsAddr *list){
	RefsAddr *aux = list;
	
	while(aux != NULL){
		RefsAddr *next_node = aux->next;
		free(aux);
		aux = next_node;
	}
}

// free the define list
void freemac(MacroList *list){
	MacroList *aux = list;
	
	while(aux != NULL){
		MacroList *next_node = aux->next;
		if(aux->pnames != NULL)
			free(aux->pnames);
		free(aux->pvalues);
		free(aux->content);
		free(aux);
		aux = next_node;
	}
}

#endif
