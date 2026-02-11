#ifndef _ASTLIB_H_
#define _ASTLIB_H_

#ifndef _INC_STDIO
#include <stdio.h>
#endif
#ifndef _INC_STDLIB
#include <stdlib.h>
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
#ifndef _INC_STRING
#include <string.h>
#endif

const char *input;
const char *input_save;
bool is_asm_proc = false;

typedef enum {
	NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_AND_BIT,
    NODE_OR_BIT,
    NODE_XOR_BIT,
    NODE_NOT_BIT,
    NODE_NUM,
    NODE_IDENT,
    NODE_OR,
    NODE_AND,
    NODE_EQUAL,
    NODE_DIFF,
    NODE_LESS,
    NODE_GREAT,
    NODE_LESS_EQ,
    NODE_GREAT_EQ,
    NODE_MOD,
    NODE_SHT_LEFT,
    NODE_SHT_RIGHT,
    NODE_NOT,
    NODE_EXP,
    NODE_ASSIGN
} NodeType;

typedef struct AST {
    NodeType type;
    int value;          // usado se NODE_NUM
    char *ident;        // usado se NODE_IDENT
    struct AST *left;
    struct AST *right;
} AST;

const char* instruction[] = {
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"AND",
	"OR",
	"XOR",
	"NOT"
};

AST *parse_logical_or();
AST *parse_logical_and();
AST *parse_relational();
AST *parse_add();
AST *parse_mul();
AST *parse_unary();
AST *parse_primary();
int gen(AST*, bool*, int);

AST *new_num(int value) {
    AST *n = malloc(sizeof(AST));
    n->type = NODE_NUM;
    n->value = value;
    n->ident = NULL;
    n->left = n->right = NULL;
    return n;
}

AST *new_ident(char *name) {
    AST *n = malloc(sizeof(AST));
    n->type = NODE_IDENT;
    n->value = 0;
    n->ident = name;
    n->left = n->right = NULL;
    return n;
}

AST *new_op(NodeType type, AST *l, AST *r) {
    AST *n = malloc(sizeof(AST));
    n->type = type;
    n->value = 0;
    n->ident = NULL;
    n->left = l;
    n->right = r;
    return n;
}


void skip_spaces() {
    while (*input == ' ' || *input == '\t')
        input++;
}

int is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
            c == '_' || c == '.';
}

int is_alnum(char c) {
    return is_alpha(c) || (c >= '0' && c <= '9');
}

int is_hexa(const char* c){
	return ((c[0] == 'H' || c[0] == 'h') && c[1] == '\'');
}


int parse_number() {
	const char* start = NULL;
    char *end = NULL;
    char buffer[64];
    
    int value = 0;
    int base = 10;
	int len = strcspn(input, "H");
	
    // $FF
    if (*input == '$') {
        input++;
        start = input;
        base = 16;
        value = strtol(start, &end, base);
        input = end;
    }
    // 0xFF
    else if (input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) {
        input += 2;
        start = input;
        base = 16;
        value = strtol(start, &end, base);
        input = end;
    }
    // H'FF'
    else if ((input[0] == 'H' || input[0] == 'h') && input[1] == '\'') {
        input += 2;
        base = 16;
        len = strcspn(input, "'");
        memcpy(buffer, input, len);
        buffer[len] = '\0';
        start = buffer;
        input += len + 1;
        value = strtol(start, &end, base);
    }
    // FFh
	else if(*(input + len) == 'H'){
    	base = 16;
    	memcpy(buffer, input, len);
    	buffer[len] = '\0';
    	start = buffer;
    	input += len + 1;
    	value = strtol(start, &end, base);
	}else{
		value = strtol(input, &end, base);
		input = end;
	}
	
    return value;
}


char *parse_ident() {
    const char *start = input;
    while (is_alnum(*input))
        input++;

    int len = input - start;
    char *name = malloc(len + 1);
    memcpy(name, start, len);
    name[len] = '\0';

    return name;
}



AST *parse_primary() {
    skip_spaces();

    // parênteses
    if (*input == '(') {
        input++; // '('
        AST *node = parse_logical_or();
        skip_spaces();
        input++; // ')'
        return node;
    }

    // identificador
    if (is_alpha(*input) && !is_hexa(input)) {
        char *name = parse_ident();
        return new_ident(name);
    }

    // número
    int value = parse_number();
    return new_num(value);
}

AST *parse_unary() {
    skip_spaces();

    if (*input == '~') {
        input++;
        return new_op(NODE_NOT_BIT, NULL, parse_unary());
    }

    if (*input == '!' && *(input+1) != '=') {
        input++;
        return new_op(NODE_NOT, NULL, parse_unary());
    }

    return parse_primary();
}


AST *parse_mul() {
    AST *node = parse_unary();

    while (1) {
        skip_spaces();
		
		if (*input == '*' && *(input+1) == '*'){
			input += 2;
			node = new_op(NODE_EXP, node, parse_unary());
		} else if (*input == '*') {
            input++;
            node = new_op(NODE_MUL, node, parse_unary());
        } else if (*input == '/') {
            input++;
            node = new_op(NODE_DIV, node, parse_unary());
        } else if (*input == '%') {
            input++;
            node = new_op(NODE_MOD, node, parse_unary());
        } else if (*input == '<' && *(input+1) == '<') {
            input += 2;
            node = new_op(NODE_SHT_LEFT, node, parse_unary());
        } else if (*input == '>' && *(input+1) == '>') {
            input += 2;
            node = new_op(NODE_SHT_RIGHT, node, parse_unary());
        } else if (*input == '&' && *(input+1) != '&') {
            input++;
            node = new_op(NODE_AND_BIT, node, parse_unary());
        } else {
            break;
        }
    }

    return node;
}



AST *parse_add() {
    AST *node = parse_mul();

    while (1) {
        skip_spaces();

        if (*input == '+') {
            input++;
            node = new_op(NODE_ADD, node, parse_mul());
        } else if (*input == '-') {
            input++;
            node = new_op(NODE_SUB, node, parse_mul());
        } else if (*input == '|' && *(input+1) != '|') {
            input++;
            node = new_op(NODE_OR_BIT, node, parse_mul());
        } else if (*input == '^') {
            input++;
            node = new_op(NODE_XOR_BIT, node, parse_mul());
        } else {
            break;
        }
    }

    return node;
}

AST *parse_relational(){
	AST *node = parse_add();
	
	while(1){
		skip_spaces();
		
		if (*input == '=' && *(input+1) == '=') {
			input += 2;
			node = new_op(NODE_EQUAL, node, parse_add());
		} else if(*input == '!' && *(input+1) == '=') {
			input += 2;
			node = new_op(NODE_DIFF, node, parse_add());
		} else if(*input == '<' && *(input+1) == '=') {
			input += 2;
			node = new_op(NODE_LESS_EQ, node, parse_add());
		} else if(*input == '>' && *(input+1) == '=') {
			input += 2;
			node = new_op(NODE_GREAT_EQ, node, parse_add());
		} else if(*input == '<') {
			input++;
			node = new_op(NODE_LESS, node, parse_add());
		} else if(*input == '>') {
			input++;
			node = new_op(NODE_GREAT, node, parse_add());
		} else {
			break;
		}
	}
	
	return node;
}

AST *parse_logical_and(){
	AST *node = parse_relational();
	
	while(1){
		skip_spaces();
		
		if (*input == '&' && *(input+1) == '&') {
			input += 2;
			node = new_op(NODE_AND, node, parse_relational());
		} else {
			break;
		}
	}
	
	return node;
}

AST *parse_logical_or(){
	AST *node = parse_logical_and();

	while(1){
		skip_spaces();
	
		if (*input == '|' && *(input+1) == '|') {
			input += 2;
			node = new_op(NODE_OR, node, parse_logical_and());
		} else {
			break;
		}
	}
	
	return node;	
}

AST *parse_assign(){
	AST *node = parse_logical_or();

	while(1){
		skip_spaces();
	
		if (*input == '=' && *(input+1) != '=') {
			input++;
			node = new_op(NODE_ASSIGN, node, parse_logical_or());
		} else {
			break;
		}
	}
	
	return node;	
}


AST *parse(const char *str) {
    input = str;
    input_save = str;
    return parse_assign();
}

void assembly(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
    printf("LD R%d\n", rx++);
    gen(node->left, state, rx);
    printf("%s R%d\n", instruction[type], --rx);
}

int gen(AST *node, bool* state, int rx) {
	// TODO: Criar mais operações e fazer parsing de hexadecimais
    switch (node->type) {
        case NODE_NUM:	{
        	printf("STD %d\n", node->value);
			break;
		}
        
        case NODE_ADD:		{
        	assembly(node, state, rx, NODE_ADD);
			break;
		}
        case NODE_SUB:		{
        	assembly(node, state, rx, NODE_SUB);
			break;
		}
        case NODE_MUL:		{
        	assembly(node, state, rx, NODE_MUL);
			break;
		}
        case NODE_DIV:		{
        	assembly(node, state, rx, NODE_DIV);
			break;
		}
		case NODE_AND_BIT: 	{
			assembly(node, state, rx, NODE_AND_BIT);
			break;
		}
		case NODE_OR_BIT: 	 {
			assembly(node, state, rx, NODE_OR_BIT);
			break;
		}
		case NODE_XOR_BIT: 	 {
			assembly(node, state, rx, NODE_XOR_BIT);
			break;
		}
        /*
        case NODE_NOT_BIT: 	 return ~gen(node->right, state);
        case NODE_OR: 	 	 return gen(node->left, state) || gen(node->right, state);
        case NODE_AND: 	 	 return gen(node->left, state) && gen(node->right, state);
        case NODE_EQUAL: 	 return gen(node->left, state) == gen(node->right, state);
        case NODE_DIFF:  	 return gen(node->left, state) != gen(node->right, state);
        case NODE_LESS:  	 return gen(node->left, state) < gen(node->right, state);
        case NODE_GREAT:   	 return gen(node->left, state) > gen(node->right, state);
        case NODE_LESS_EQ: 	 return gen(node->left, state) <= gen(node->right, state);
        case NODE_GREAT_EQ:  return gen(node->left, state) >= gen(node->right, state);
        case NODE_MOD: 		 return gen(node->left, state) % gen(node->right, state);
        case NODE_SHT_LEFT:  return gen(node->left, state) << gen(node->right, state);
        case NODE_SHT_RIGHT: return gen(node->left, state) >> gen(node->right, state);
        case NODE_NOT: 		 return !gen(node->right, state);
        case NODE_EXP: 		 return (int)pow(gen(node->left, state), gen(node->right, state));
        */
        case NODE_ASSIGN: 	 {
        	gen(node->right, state, rx);
        	printf("PUSHD\n");
        	
        	if(node->left->ident) 
				printf("STD %s::8\n", node->left->ident);
        	else
        		printf("STD 0x%03X::8\n", node->left->value);
					
        	printf("OUT P0\n");
        	
        	if(node->left->ident)
        		printf("STD %s::0\n", node->left->ident);
        	else
        		printf("STD 0x%03X::0\n", node->left->value);
        		
        	printf("OUT P1\n");
        	
        	printf("POPD\n");
        	printf("OUT P2\n");
			break;
		}
        case NODE_IDENT: {
        	printf("STD %s::8\n", node->ident);
        	printf("OUT P0\n");
        	printf("STD %s::0\n", node->ident);	
        	printf("OUT P1\n");
        	printf("IN P2\n");
        	break;
		}
    }
    return 0;
}


void free_ast(AST *node) {
    if (node == NULL)
        return;

    free_ast(node->left);
    free_ast(node->right);

    if (node->type == NODE_IDENT && node->ident != NULL) {
        free(node->ident);
    }

    free(node);
}

#endif
