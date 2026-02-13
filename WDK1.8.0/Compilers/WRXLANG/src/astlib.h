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
    // especiais
    TOK_EOF,
    TOK_NUM,
    TOK_IDENT,

    // aritméticos
    TOK_PLUS, TOK_MINUS,
    TOK_MUL, TOK_DIV, TOK_MOD,
    TOK_EXP,              // **

    // shifts
    TOK_SHL, TOK_SHR,     // << >>

    // bitwise
    TOK_AND_BIT,          // &
    TOK_OR_BIT,           // |
    TOK_XOR_BIT,          // ^
    TOK_NOT_BIT,          // ~

    // lógicos
    TOK_NOT,              // !
    TOK_AND,              // &&
    TOK_OR,               // ||

    // relacionais
    TOK_EQUAL_EQUAL,      // ==
    TOK_NOT_EQUAL,        // !=
    TOK_LESS, TOK_GREATER,
    TOK_LESS_EQ, TOK_GREATER_EQ,

    // atribuição
    TOK_ASSIGN,           // =

    // pontuação
    TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE,	TOK_RBRACE,

    TOK_SEMI,

    // keywords
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE
} TokenType;


typedef struct {
    TokenType type;
    int value;
    char text[64];
} Token;

#define MAX_TOKENS 8192

Token tokens[MAX_TOKENS];
int tok_count = 0;
int tok_pos = 0;

Token* peek() { return &tokens[tok_pos]; }
Token* next() { return &tokens[tok_pos++]; }

bool match(TokenType t) {
    if (peek()->type == t) {
        tok_pos++;
        return true;
    }
    return false;
}


void add_token(TokenType type, int value, const char* text) {
    tokens[tok_count].type = type;
    tokens[tok_count].value = value;
    if (text) strcpy(tokens[tok_count].text, text);
    tok_count++;
}

void skip_spaces(char **p) {
    while (**p) {
        // espaços
        if (isspace(**p)) { (*p)++; continue; }

        // comentário de linha
        if ((*p)[0]=='/' && (*p)[1]=='/') {
            (*p)+=2;
            while(**p && **p!='\n') (*p)++;
            continue;
        }

        // comentário em blocos
        if ((*p)[0]=='/' && (*p)[1]=='*') {
            (*p)+=2;
            while(**p && !((*p)[0]=='*' && (*p)[1]=='/')) (*p)++;
            if (**p) (*p)+=2;
            continue;
        }

        break;
    }
}

/*
void skip_spaces() {
    while (*input == ' ' || *input == '\t')
        input++;
}
*/


int is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
            c == '_' || c == '.';
}

int is_alnum(char c) {
    return is_alpha(c) || (c >= '0' && c <= '9');
}

int is_hexa(const char* c){
	return ((c[0] == 'H' || c[0] == 'h') && c[1] == '\'') || c[0] == '$' || (c[0] == '0' && c[1] == 'x');
}

typedef enum {
	R0, R1, R2, R3, R4, R5, R6, R7,
	LITERAL,
	REGISTER
} OperandType;

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
    NODE_EQUAL,
    NODE_DIFF,
    NODE_LESS,
    NODE_GREAT,
    NODE_LESS_EQ,
    NODE_GREAT_EQ,
    NODE_MOD,
    NODE_NOT,
    NODE_EXP,
    NODE_NUM,
    NODE_IDENT,
    NODE_OR,
    NODE_AND,
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

typedef enum {
    STMT_EXPR,
    STMT_IF,
    STMT_WHILE
} StmtType;

typedef struct Stmt {
    StmtType type;
    AST *expr;
    struct Stmt *then_branch;
    struct Stmt *else_branch;
    struct Stmt *body;
    struct Stmt *next;
} Stmt;


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
	"SHR",
	"BT"
};

#define NODE_COMP		NODE_EQUAL
#define FALSE_INDEX		0
#define TRUE_INDEX		1 	
const char* cond_state[] = {
	"CDR",
	"STD 1"
};

void lex(char *src) {
    char *p = src;
    tok_count = 0;

    while (1) {
        skip_spaces(&p);
        if (*p == 0) break;

        // ---------------------------
        // números (texto bruto)
        // suporta: $FF, 0xFF, FFh, H'FF', 'A'
        // ---------------------------
        if (isdigit(*p) || is_hexa(p)) {
            char buf[64];
            int i = 0;

            while (*p &&
                   !isspace(*p) &&
                   !strchr("+-*/%&|^~!=<>();", *p))
            {
                buf[i++] = *p++;
            }
            buf[i] = 0;

            add_token(TOK_NUM, 0, buf);
            continue;
        }

        // ---------------------------
        // identificador / keyword
        // ---------------------------
        if (is_alpha(*p)) {
            char buf[64];
            int i = 0;

            while (is_alnum(*p))
                buf[i++] = *p++;
            buf[i] = 0;

            if (!strcmp(buf,"if")) add_token(TOK_IF,0,NULL);
            else if (!strcmp(buf,"else")) add_token(TOK_ELSE,0,NULL);
            else if (!strcmp(buf,"while")) add_token(TOK_WHILE,0,NULL);
            else add_token(TOK_IDENT,0,buf);

            continue;
        }

        // ---------------------------
        // operadores multi-char
        // ---------------------------
        if (p[0]=='=' && p[1]=='='){ add_token(TOK_EQUAL_EQUAL,0,NULL); p+=2; continue; }
        if (p[0]=='!' && p[1]=='='){ add_token(TOK_NOT_EQUAL,0,NULL); p+=2; continue; }
        if (p[0]=='<' && p[1]=='='){ add_token(TOK_LESS_EQ,0,NULL); p+=2; continue; }
        if (p[0]=='>' && p[1]=='='){ add_token(TOK_GREATER_EQ,0,NULL); p+=2; continue; }
        if (p[0]=='&' && p[1]=='&'){ add_token(TOK_AND,0,NULL); p+=2; continue; }
        if (p[0]=='|' && p[1]=='|'){ add_token(TOK_OR,0,NULL); p+=2; continue; }
        if (p[0]=='<' && p[1]=='<'){ add_token(TOK_SHL,0,NULL); p+=2; continue; }
        if (p[0]=='>' && p[1]=='>'){ add_token(TOK_SHR,0,NULL); p+=2; continue; }
        if (p[0]=='*' && p[1]=='*'){ add_token(TOK_EXP,0,NULL); p+=2; continue; }

        // ---------------------------
        // single-char
        // ---------------------------
        switch(*p){
            case '+': add_token(TOK_PLUS,0,NULL); break;
            case '-': add_token(TOK_MINUS,0,NULL); break;
            case '*': add_token(TOK_MUL,0,NULL); break;
            case '/': add_token(TOK_DIV,0,NULL); break;
            case '%': add_token(TOK_MOD,0,NULL); break;

            case '&': add_token(TOK_AND_BIT,0,NULL); break;
            case '|': add_token(TOK_OR_BIT,0,NULL); break;
            case '^': add_token(TOK_XOR_BIT,0,NULL); break;
            case '~': add_token(TOK_NOT_BIT,0,NULL); break;
            case '!': add_token(TOK_NOT,0,NULL); break;

            case '<': add_token(TOK_LESS,0,NULL); break;
            case '>': add_token(TOK_GREATER,0,NULL); break;

            case '=': add_token(TOK_ASSIGN,0,NULL); break;

            case '(': add_token(TOK_LPAREN,0,NULL); break;
            case ')': add_token(TOK_RPAREN,0,NULL); break;
            case ';': add_token(TOK_SEMI,0,NULL); break;
            case '{': add_token(TOK_LBRACE,0,NULL); break;
			case '}': add_token(TOK_RBRACE,0,NULL); break;

        }
        p++;
    }

    add_token(TOK_EOF,0,NULL);
}


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
    AST *n = calloc(1, sizeof(AST));
    n->type = NODE_NUM;
    n->value = value;
    n->ident = NULL;
    n->left = n->right = NULL;
    return n;
}

AST *new_ident(char *name) {
    AST *n = calloc(1, sizeof(AST));
    n->type = NODE_IDENT;
    n->value = 0;
    n->ident = name;
    n->left = n->right = NULL;
    return n;
}

AST *new_op(NodeType type, AST *l, AST *r) {
    AST *n = calloc(1, sizeof(AST));
    n->type = type;
    n->value = 0;
    n->ident = NULL;
    n->left = l;
    n->right = r;
    return n;
}


int parse_number(char *input) {
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
	}
	// 'A'
	else if(*input == '\''){
			input++;
			value = (int)*input;
			input += 2;
 	}
 	// 10
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
    char *name = calloc(len + 1, sizeof(char));
    memcpy(name, start, len);
    name[len] = '\0';

    return name;
}


AST *parse_primary() {
    Token *t = peek();

    if (match(TOK_LPAREN)) {
        AST *n = parse_assign();
        match(TOK_RPAREN);
        return n;
    }
    
    if (match(TOK_IDENT))
        return new_ident(strdup(t->text));
    
    if (match(TOK_NUM)){
    	t->value = parse_number(t->text);
    	return new_num(t->value);
	}
        
    return NULL;
}

AST *parse_unary() {
    if (match(TOK_NOT_BIT))
        return new_op(NODE_NOT_BIT, NULL, parse_unary());

    if (match(TOK_NOT))
        return new_op(NODE_NOT, NULL, parse_unary());

    if (match(TOK_MUL))
        return new_op(NODE_POINTER, NULL, parse_unary());

    return parse_primary();
}

AST *parse_mul() {
    AST *node = parse_unary();

    while (1) {
        if (match(TOK_EXP))
            node = new_op(NODE_EXP, node, parse_unary());

        else if (match(TOK_MUL))
            node = new_op(NODE_MUL, node, parse_unary());

        else if (match(TOK_DIV))
            node = new_op(NODE_DIV, node, parse_unary());

        else if (match(TOK_MOD))
            node = new_op(NODE_MOD, node, parse_unary());

        else if (match(TOK_SHL))
            node = new_op(NODE_SHT_LEFT, node, parse_unary());

        else if (match(TOK_SHR))
            node = new_op(NODE_SHT_RIGHT, node, parse_unary());

        else if (match(TOK_AND_BIT))
            node = new_op(NODE_AND_BIT, node, parse_unary());

        else
            break;
    }

    return node;
}

AST *parse_add() {
    AST *node = parse_mul();

    while (1) {
        if (match(TOK_PLUS))
            node = new_op(NODE_ADD, node, parse_mul());

        else if (match(TOK_MINUS))
            node = new_op(NODE_SUB, node, parse_mul());

        else if (match(TOK_OR_BIT))
            node = new_op(NODE_OR_BIT, node, parse_mul());

        else if (match(TOK_XOR_BIT))
            node = new_op(NODE_XOR_BIT, node, parse_mul());

        else
            break;
    }

    return node;
}

AST *parse_relational() {
    AST *node = parse_add();

    while (1) {
        if (match(TOK_EQUAL_EQUAL))
            node = new_op(NODE_EQUAL, node, parse_add());

        else if (match(TOK_NOT_EQUAL))
            node = new_op(NODE_DIFF, node, parse_add());

        else if (match(TOK_LESS_EQ))
            node = new_op(NODE_LESS_EQ, node, parse_add());

        else if (match(TOK_GREATER_EQ))
            node = new_op(NODE_GREAT_EQ, node, parse_add());

        else if (match(TOK_LESS))
            node = new_op(NODE_LESS, node, parse_add());

        else if (match(TOK_GREATER))
            node = new_op(NODE_GREAT, node, parse_add());

        else
            break;
    }

    return node;
}

AST *parse_logical_and() {
    AST *node = parse_relational();

    while (match(TOK_AND))
        node = new_op(NODE_AND, node, parse_relational());

    return node;
}

AST *parse_logical_or() {
    AST *node = parse_logical_and();

    while (match(TOK_OR))
        node = new_op(NODE_OR, node, parse_logical_and());

    return node;
}

AST *parse_assign() {
    AST *node = parse_logical_or();

    if (match(TOK_ASSIGN))
        node = new_op(NODE_ASSIGN, node, parse_assign());

    return node;
}


/*

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
*/

AST *parse(const char *str) {
    input = str;
    input_save = str;
    return parse_assign();
}

Stmt* parse_statement();

Stmt* parse_block() {
    // já consumiu '{'
    Stmt *head = NULL;
    Stmt **curr = &head;

    while (peek()->type != TOK_RBRACE &&
           peek()->type != TOK_EOF)
    {
        *curr = parse_statement();
        if(*curr == NULL)
        	return NULL;
        curr = &((*curr)->next);
    }

    if(!match(TOK_RBRACE)){
    	printf("error 2 => its missing '}'\n");
    	return NULL;
	}
    return head;
}

Stmt* parse_if() {
    match(TOK_IF);
    match(TOK_LPAREN);
    AST *cond = parse_assign();
    match(TOK_RPAREN);

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_IF;
    s->expr = cond;
    s->then_branch = parse_statement();
    
	if(s->then_branch == NULL)
		return NULL;

    if (match(TOK_ELSE))
        s->else_branch = parse_statement();
    else
        s->else_branch = NULL;

	s->next = NULL;
    return s;
}


Stmt* parse_while() {
    match(TOK_WHILE);
    match(TOK_LPAREN);
    AST *cond = parse_assign();
    match(TOK_RPAREN);

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_WHILE;
    s->expr = cond;
    s->body = parse_statement();
    
    s->next = NULL;
    return s;
}

Stmt* parse_expr_stmt() {
    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_EXPR;
    s->expr = parse_assign();
    
	if(!match(TOK_SEMI)){
		printf("error 1 => its missing ';'\n");
		return NULL;
	} 	
    
    s->next = NULL;
    return s;
}

Stmt* parse_statement() {
	if (peek()->type == TOK_LBRACE) {
        if(!match(TOK_LBRACE)){
        	printf("error 3 => its missing '{'\n");
        	return NULL;
		}
        return parse_block();
    }
    
    if (peek()->type == TOK_IF)
        return parse_if();

    if (peek()->type == TOK_WHILE)
        return parse_while();

    return parse_expr_stmt();
}


void gen_math(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
	printf("LD R%d\n", rx);
	if(type != NODE_NOT_BIT){
		gen(node->left, state, ++rx);
		rx--;
	}
    printf("%s R%d\n", math_operation[type], rx);
}

void gen_math_exp(AST *node, bool* state, int rx){
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
}

void gen_move(AST *node, int bit, OperandType type, OperandType reg){
	if(type == LITERAL){
		(node->ident) ?
			printf("STD %s::%d\n", node->ident, bit) 	:
			printf("STD 0x%03X::%d\n", node->value & 0xFFF, bit);	
	}else{
		printf("STL R%d\n", reg);
	}
}

void gen_shift(AST *node, bool* state, int rx, int type){
	if(node->right->type == NODE_NUM){
    	gen(node->left, state, rx);
    	if(node->right->value != 0)
    		printf("%s %d\n", math_operation[type], node->right->value);
	}else{
		printf("STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
	    printf("IDC\n");
	    		
		gen(node->right, state, rx);
    	printf("LD R%d\n", rx++);
    	printf("DECR\n");
    	gen(node->left, state, rx);
    	printf("%s 1\n", math_operation[type]);
    	printf("DECR\n");
    	printf("JC @-2\n");
	}
}

void gen_logic(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
	if(type != NODE_NOT){
		printf("JZ @+4\n");
    	printf("STD 1\n");
    	printf("LD R%d\n", rx++);
    	gen(node->left, state, rx);
    	printf("JZ @+4\n");
    	printf("STD 1\n");
    	printf("%s R%d\n", math_operation[type], --rx);	
	}else{
		printf("JZ @+5\n");
		printf("CDR\n");
		printf("JP @+4\n");
		printf("STD 1\n");
		printf("LD R%d\n", rx);
	}
}

void gen_addr(AST *node){
	gen_move(node, HIGH_PART, LITERAL, 0);
	printf("OUT P0\n");
    gen_move(node, LOW_PART, LITERAL, 0);
    printf("OUT P1\n");
}

bool is_assigning = false;
void gen_io_write(AST *node, bool* state, int rx){
	gen(node->right, state, rx);
	if(node->left->value > 0xFFF){
		printf("OUT P%d\n", (node->left->value & 0x7));
	}else{
		printf("PUSHD\n");
	    is_assigning = true;
		gen(node->left, state, rx);
		is_assigning = false;
	    printf("POPD\n");
	    printf("OUT P2\n");	
	} 
}

void gen_io_read(AST *node){	
	gen_addr(node);
	printf("IN P2\n");
}

void gen_io_pointer(AST *node, bool* state, int rx, int number){
	static int depth = 0, depth_a = 0;
	
	bool increment = depth == 0 && !is_assigning || depth > 0 && is_assigning;
	bool idc_config = increment && depth == 1;
	depth++;
	if(idc_config){
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
		//node->right->value = number;
		gen_move(node->right, HIGH_PART, LITERAL, 0);
		printf("OUT P0\n");
	}
    		
    if(increment)
		printf("IN P2\n");
				
	if(--depth_a > 0){
		printf("PUSHD\n");
	    printf("INCR\n");
	    printf("IN P2\n");
	}else{
		depth = depth_a;
	}	
}

void gen_branch_eqdiff(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	printf("JZ @+%d\n", (type == NODE_EQUAL) ? off_true : off_true + 1);
    printf("%s\n", (type == NODE_EQUAL) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    printf("JP @+%d\n", (type == NODE_EQUAL) ? off_false : off_false - 1);
    printf("%s\n", (type == NODE_EQUAL) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_geqlt(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	printf("JC @+%d\n", (type == NODE_GREAT_EQ) ? off_true : off_true + 1);
    printf("%s\n", (type == NODE_GREAT_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    printf("JP @+%d\n", (type == NODE_GREAT_EQ) ? off_false : off_false - 1);
    printf("%s\n", (type == NODE_GREAT_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_leqgt(int type, const char* state[]){
	int off_true_1 = 6;
	int off_true_2 = 4;
	int off_false = 5;
	
	printf("JC @+%d\n", (type == NODE_LESS_EQ) ? off_true_1 : off_true_1 - 1);
    printf("%s\n", (type == NODE_LESS_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
    printf("JP @+%d\n", (type == NODE_LESS_EQ) ? off_false : off_false + 1);
    printf("JZ @-%d\n", (type == NODE_LESS_EQ) ? off_true_2 : off_true_2 - 1);
    printf("%s\n", (type == NODE_LESS_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
}

void gen_relational(AST *node, bool* state, int rx, int type, const char* cond[]){
	gen_math(node, state, rx, NODE_COMP);
	if(type == NODE_EQUAL || type == NODE_DIFF)
		gen_branch_eqdiff(type, cond);
	else if(type == NODE_GREAT_EQ || type == NODE_LESS)
		gen_branch_geqlt(type, cond);
	else{
		gen_branch_leqgt(type, cond);
	}	
}

int gen(AST *node, bool* state, int rx) {
	if(!node) return -1;
	
	static int number = 0;
    switch (node->type) {
        case NODE_NUM:		 {
        	number = node->value;
        	gen_move(node, LOW_PART, LITERAL, 0);
			break;
		}
        case NODE_ADD:		 {
        	gen_math(node, state, rx, NODE_ADD);
			break;
		}
        case NODE_SUB:		 {
        	gen_math(node, state, rx, NODE_SUB);
			break;
		}
        case NODE_MUL:		 {
        	gen_math(node, state, rx, NODE_MUL);
			break;
		}
        case NODE_DIV:		 {
        	gen_math(node, state, rx, NODE_DIV);
			break;
		}
		case NODE_MOD: 		 {
			gen_math(node, state, rx, NODE_DIV);
			gen_move(node, 0, REGISTER, R0);
			break;
		}
		case NODE_AND_BIT: 	 {
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
			gen_math(node, state, rx, NODE_NOT_BIT);
			break;
		}
		case NODE_SHT_LEFT:  {
			gen_shift(node, state, rx, NODE_SHT_LEFT);
			break;
		}
        case NODE_SHT_RIGHT: {
        	gen_shift(node, state, rx, NODE_SHT_RIGHT);
			break;
		}
		case NODE_OR: 	 	 {
			gen_logic(node, state, rx, NODE_OR_BIT);
			break;
		}
        case NODE_AND: 	 	 {
        	gen_logic(node, state, rx, NODE_AND_BIT);
			break;
		}
		case NODE_NOT: 		 {
			gen_logic(node, state, rx, NODE_NOT);
			break;
		}
		case NODE_EXP: 		 {
			gen_math_exp(node, state, rx);
			break;
		}
        case NODE_EQUAL: 	 {
        	gen_relational(node, state, rx, NODE_EQUAL, cond_state);
			break;
		}
        case NODE_DIFF:  	 {
        	gen_relational(node, state, rx, NODE_DIFF, cond_state);
			break;
		}
        case NODE_LESS:  	 {
        	gen_relational(node, state, rx, NODE_LESS, cond_state);
			break;
		}
        case NODE_GREAT:   	 {
        	gen_relational(node, state, rx, NODE_GREAT, cond_state);
			break;
		}
        case NODE_LESS_EQ: 	 {
        	gen_relational(node, state, rx, NODE_LESS_EQ, cond_state);
			break;
		}
        case NODE_GREAT_EQ:  {
        	gen_relational(node, state, rx, NODE_GREAT_EQ, cond_state);
			break;
		}
        case NODE_ASSIGN: 	 {
        	gen_io_write(node, state, rx);
			break;
		}
        case NODE_IDENT:	 {
        	gen_io_read(node);
        	break;
		}
		case NODE_POINTER:	 {
			gen_io_pointer(node, state, rx, number);
			break;
		}
    }
    return 0;
}

int gen_stmt(Stmt *s) {
	if(!s) return -1;
	
	bool st=false;
	//int result = 0;
	while(s) {
	    switch(s->type) {
		    case STMT_EXPR: {
		        gen(s->expr, &st, 0);
		        //printf("\nresult STMT_EXPR: %d\n", result);
		        break;
		    }
		
			case STMT_IF: {
			    int lbl_else = label_count++;
			    int lbl_end  = s->else_branch ? label_count++ : lbl_else;
			
			    gen(s->expr, &st, 0);
			
			    if (s->else_branch) {
			        printf("JZ else_%d\n", lbl_else);
			        gen_stmt(s->then_branch);
			        printf("JP endif_%d\n", lbl_end);
			        printf("else_%d:\n", lbl_else);
			        gen_stmt(s->else_branch);
			        printf("endif_%d:\n", lbl_end);
			    } else {
			        printf("JZ endif_%d\n", lbl_else);
			        gen_stmt(s->then_branch);
			        printf("endif_%d:\n", lbl_else);
			    }
			    break;
			}

		
		    case STMT_WHILE:{
		    	printf("; WHILE start\n");
		        break;
			}
	        
	    }
	
	    s = s->next;
	}
    return 0;
}

void compile(char *source) {
    lex(source);
    tok_pos = 0;

    Stmt *head = NULL;
    Stmt **curr = &head;

    while (peek()->type != TOK_EOF) {
        *curr = parse_statement();
        if(*curr == NULL) 	return;
        curr = &((*curr)->next);
    }

    int result = gen_stmt(head);
    //printf("\nresult GEN_STMT = %d\n", result);
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
