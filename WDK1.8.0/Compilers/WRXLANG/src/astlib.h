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

#define LOW_PART	0
#define HIGH_PART	8

const char *input;
const char *input_save;
bool is_asm_proc = false;
int label_count = 0;

typedef enum {
	NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_AND_BIT,
    NODE_OR_BIT,
    NODE_XOR_BIT,
    NODE_NOT_BIT,
    NODE_SHT_LEFT,
    NODE_SHT_RIGHT,
    NODE_MOD,
    NODE_NOT,
    NODE_EXP,
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
    NODE_ASSIGN,
    NODE_POINTER
} NodeType;

typedef struct AST {
    NodeType type;
    int value;          // usado se NODE_NUM
    char *ident;        // usado se NODE_IDENT
    struct AST *left;
    struct AST *right;
} AST;

const char* math_operation[] = {
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"AND",
	"OR",
	"XOR",
	"NOT",
	"SHL",
	"SHR"
};

AST *parse_assign();
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
	else if(*(input + len) == 'H' || *(input + len) == 'h'){
    	base = 16;
    	memcpy(buffer, input, len);
    	buffer[len] = '\0';
    	start = buffer;
    	input += len + 1;
    	value = strtol(start, &end, base);
	}else if(*input == '\''){
			input++;
			value = (int)*input;
			input += 2;
 	}
	else{
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
        AST *node = parse_assign();
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

	if (*input == '!') {
        input++;
        return new_op(NODE_NOT, NULL, parse_unary());
    }
    
    if(*input == '*'){
    	input++;
    	return new_op(NODE_POINTER, NULL, parse_unary());
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

void gen_math(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
    printf("LD R%d\n", rx++);
    gen(node->left, state, rx);
    printf("%s R%d\n", math_operation[type], --rx);
}

void gen_move(AST *node, int bit){
	(node->ident) ?
		printf("STD %s::%d\n", node->ident, bit) 	:
		printf("STD 0x%03X::%d\n", node->value & 0xFFF, bit);
}

void gen_addr(AST *node){
	gen_move(node, HIGH_PART);
	printf("OUT P0\n");
    gen_move(node, LOW_PART);
    printf("OUT P1\n");
}

void gen_io_write(AST *node, bool* state, int rx){
	gen(node->right, state, rx);
	if(node->left->value > 0xFFF){
		printf("OUT P%d\n", (node->left->value & 0x7));
	}else{
		printf("PUSHD\n");
	    gen_addr(node->left);
	    printf("POPD\n");
	    printf("OUT P2\n");	
	} 
}

void gen_io_read(AST *node){	
	gen_addr(node);
    printf("IN P2\n");
}

int gen(AST *node, bool* state, int rx) {
	static int depth = 0, depth_a = 0;
    switch (node->type) {
        case NODE_NUM:		{
        	gen_move(node, LOW_PART);
			break;
		}
        case NODE_ADD:		{
        	gen_math(node, state, rx, NODE_ADD);
			break;
		}
        case NODE_SUB:		{
        	gen_math(node, state, rx, NODE_SUB);
			break;
		}
        case NODE_MUL:		{
        	gen_math(node, state, rx, NODE_MUL);
			break;
		}
        case NODE_DIV:		{
        	gen_math(node, state, rx, NODE_DIV);
			break;
		}
		case NODE_MOD: 		{
			gen_math(node, state, rx, NODE_DIV);
			printf("STL R0\n");
			break;
		}
		case NODE_AND_BIT: 	{
			gen_math(node, state, rx, NODE_AND_BIT);
			break;
		}
		case NODE_OR_BIT: 	 {
			gen_math(node, state, rx, NODE_OR_BIT);
			break;
		}
		case NODE_XOR_BIT: 	 {
			gen_math(node, state, rx, NODE_XOR_BIT);
			break;
		}
		case NODE_NOT_BIT: 	 {
			gen(node->right, state, rx);
			printf("LD R%d\n", rx);
			printf("%s R%d\n", math_operation[NODE_NOT_BIT], rx);
			break;
		}
		case NODE_OR: 	 	 {
			gen(node->right, state, rx);
        	printf("JZ @+4\n");
        	printf("STD 1\n");
    		printf("LD R%d\n", rx++);
    		gen(node->left, state, rx);
    		printf("JZ @+4\n");
        	printf("STD 1\n");
    		printf("%s R%d\n", math_operation[NODE_OR_BIT], --rx);
			break;
		}
        case NODE_AND: 	 	 {
        	gen(node->right, state, rx);
        	printf("JZ @+4\n");
        	printf("STD 1\n");
    		printf("LD R%d\n", rx++);
    		gen(node->left, state, rx);
    		printf("JZ @+4\n");
        	printf("STD 1\n");
    		printf("%s R%d\n", math_operation[NODE_AND_BIT], --rx);
			break;
		}
		case NODE_NOT: 		 {
			gen(node->right, state, rx);
			printf("JZ @+5\n");
			printf("CDR\n");
			printf("JP @+4\n");
			printf("STD 1\n");
			printf("LD R%d\n", rx);
			break;
		}
		case NODE_EXP: 		 {
			rx++;
			printf("STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
    		printf("IDC\n");
    		
			gen(node->right, state, rx);
			printf("LD R%d\n", rx++);
			printf("DECR\n");
			printf("JC @+6\n");
			printf("STD 1\n");
			printf("JP exp_end_%d\n", label_count);
			printf("DECR\n");
    		gen(node->left, state, rx);
    		printf("JC @+4\n");
    		printf("JP exp_end_%d\n", label_count);
    		printf("LD R%d\n", rx);
    		printf("%s R%d\n", math_operation[NODE_MUL], rx);
    		printf("DECR\n");
    		printf("JC @-2\n");
    		printf("exp_end_%d:\n", label_count++);
			break;
		}
		case NODE_SHT_LEFT:  {
			if(node->right->type == NODE_NUM){
    			gen(node->left, state, rx);
    			if(node->right->value != 0)
    				printf("%s %d\n", math_operation[NODE_SHT_LEFT], node->right->value);
			}else{
				printf("STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
	    		printf("IDC\n");
	    		
				gen(node->right, state, rx);
    			printf("LD R%d\n", rx++);
    			printf("DECR\n");
    			gen(node->left, state, rx);
    			printf("%s 1\n", math_operation[NODE_SHT_LEFT]);
    			printf("DECR\n");
    			printf("JC @-2\n");
			}
			break;
		}
        case NODE_SHT_RIGHT: {
        	if(node->right->type == NODE_NUM){
    			gen(node->left, state, rx);
    			if(node->right->value != 0)
    				printf("%s %d\n", math_operation[NODE_SHT_RIGHT], node->right->value);
			}else{
				printf("STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
	    		printf("IDC\n");
	    		
				gen(node->right, state, rx);
    			printf("LD R%d\n", rx++);
    			printf("DECR\n");
    			gen(node->left, state, rx);
    			printf("%s 1\n", math_operation[NODE_SHT_RIGHT]);
    			printf("DECR\n");
    			printf("JC @-2\n");
			}
			break;
		}
        /*
        case NODE_EQUAL: 	 return gen(node->left, state) == gen(node->right, state);
        case NODE_DIFF:  	 return gen(node->left, state) != gen(node->right, state);
        case NODE_LESS:  	 return gen(node->left, state) < gen(node->right, state);
        case NODE_GREAT:   	 return gen(node->left, state) > gen(node->right, state);
        case NODE_LESS_EQ: 	 return gen(node->left, state) <= gen(node->right, state);
        case NODE_GREAT_EQ:  return gen(node->left, state) >= gen(node->right, state);
        */
        case NODE_ASSIGN: 	 {
        	gen_io_write(node, state, rx);
			break;
		}
        case NODE_IDENT:	 {
        	gen_io_read(node);
        	break;
		}
		case NODE_POINTER:	 {
			if(depth++ == 0){
				printf("STD 0x01\n");
    			printf("IDC\n");
			}
			
			depth_a = depth;
			gen(node->right, state, rx);
			
			if(depth_a != depth){
				printf("OUT P0\n");
				printf("POPD\n");
				printf("OUT P1\n");
			}else{
				printf("OUT P1\n");
				gen_move(node->right, HIGH_PART);
				printf("OUT P0\n");
			}
    		
			printf("IN P2\n");
			if(--depth_a > 0){
				printf("PUSHD\n");
	    		printf("INCR\n");
	    		printf("IN P2\n");
			}else{
				depth = depth_a;
			}
    		
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
