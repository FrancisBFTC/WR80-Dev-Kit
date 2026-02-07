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

const char *input;
bool is_asm_proc = false;

typedef enum {
    NODE_NUM,
    NODE_IDENT,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV
} NodeType;

typedef struct AST {
    NodeType type;
    int value;          // usado se NODE_NUM
    char *ident;        // usado se NODE_IDENT
    struct AST *left;
    struct AST *right;
} AST;

AST *parse_expr();
AST *parse_term();
AST *parse_factor();

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
    while (*input == ' ')
        input++;
}

int is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
            c == '_';
}

int is_alnum(char c) {
    return is_alpha(c) || (c >= '0' && c <= '9');
}



int parse_number() {
    int value = 0;
    while (*input >= '0' && *input <= '9') {
        value = value * 10 + (*input - '0');
        input++;
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



AST *parse_factor() {
    skip_spaces();

    // parênteses
    if (*input == '(') {
        input++; // '('
        AST *node = parse_expr();
        skip_spaces();
        input++; // ')'
        return node;
    }

    // identificador
    if (is_alpha(*input)) {
        char *name = parse_ident();
        return new_ident(name);
    }

    // número
    int value = parse_number();
    return new_num(value);
}



AST *parse_term() {
    AST *node = parse_factor();

    while (1) {
        skip_spaces();

        if (*input == '*') {
            input++;
            node = new_op(NODE_MUL, node, parse_factor());
        } else if (*input == '/') {
            input++;
            node = new_op(NODE_DIV, node, parse_factor());
        } else {
            break;
        }
    }

    return node;
}



AST *parse_expr() {
    AST *node = parse_term();

    while (1) {
        skip_spaces();

        if (*input == '+') {
            input++;
            node = new_op(NODE_ADD, node, parse_term());
        } else if (*input == '-') {
            input++;
            node = new_op(NODE_SUB, node, parse_term());
        } else {
            break;
        }
    }

    return node;
}



AST *parse(const char *str) {
    input = str;
    return parse_expr();
}


int eval(AST *node, bool* state) {
    switch (node->type) {
        case NODE_NUM: return node->value;
        case NODE_ADD: return eval(node->left, state) + eval(node->right, state);
        case NODE_SUB: return eval(node->left, state) - eval(node->right, state);
        case NODE_MUL: return eval(node->left, state) * eval(node->right, state);
        case NODE_DIV: return eval(node->left, state) / eval(node->right, state);
        case NODE_IDENT: {
        	int number = 0;
        	#ifdef __WR80ASM_H__
        		char* formula = strdup(node->ident);
    			*state = recursive_def(&formula, &number);
    			if(*state == false && is_asm_proc){
    				*state = calc(formula, &number, is_asm_proc);
				}
				free(formula);
    		#else
    			*state = false;
    		#endif
			return number;
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
