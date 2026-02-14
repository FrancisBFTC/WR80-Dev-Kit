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

#define MAX_TOKENS 	8192
#define LOW_PART	0
#define HIGH_PART	8

#define NODE_COMP		NODE_EQUAL
#define FALSE_INDEX		0
#define TRUE_INDEX		1 


int label_count = 0;
int tok_count = 0;
int tok_pos = 0;

int loop_begin_label = -1;
int loop_end_label = -1;

int error_code = 0;
int error_line = 0;
const char *error_msgs[];


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
//    TOK_NEXTLINE,

    // keywords
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_BREAK,
	TOK_CONTINUE,
	TOK_BYTE,
	TOK_WORD
} TokenType;


typedef struct {
    TokenType type;
    int value;
    char text[64];
    int line;
} Token;

Token tokens[MAX_TOKENS];

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
    TYPE_BYTE = 1,
    TYPE_WORD = 2
} VarType;

typedef struct {
    char name[32];
    VarType type;
} Symbol;

#define MAX_SYMBOLS 256
Symbol symtab[MAX_SYMBOLS];
int symcount = 0;

typedef enum {
    STMT_EXPR,
    STMT_IF,
    STMT_WHILE,
    STMT_BREAK,
	STMT_CONTINUE,
	STMT_DECL
} StmtType;

typedef struct Stmt {
    StmtType type;
    AST *expr;
    struct Stmt *then_branch;
    struct Stmt *else_branch;
    struct Stmt *body;
    struct Stmt *next;
    
    // --- NOVO ---
    char *ident;
    VarType vtype;
} Stmt;


int find_symbol(const char *name) {
    for (int i = 0; i < symcount; i++) {
        if (strcmp(symtab[i].name, name) == 0)
            return i;
    }
    return -1;
}

bool add_symbol(const char *name, VarType type) {
    if (find_symbol(name) != -1) {
        printf("[error] the variable '%s' already exists!\n", name);
        return false;
    }

    strcpy(symtab[symcount].name, name);
    symtab[symcount].type = type;
    symcount++;
    return true;
}


AST *parse_expression();
AST *parse_assign();
AST *parse_logical_or();
AST *parse_logical_and();
AST *parse_relational();
AST *parse_add();
AST *parse_mul();
AST *parse_unary();
AST *parse_primary();

Stmt* parse_statement();
int gen(AST*, bool*, int);
int gen_stmt(Stmt*);

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
	
const char* cond_state[] = {
	"CDR",
	"STD 1"
};

typedef enum {
    ERR_NONE = 0,
    ERR_LEX_INVALID_CHAR,

    ERR_EXPECT_SEMI,
    ERR_EXPECT_RPAREN,
    ERR_EXPECT_LPAREN,
    ERR_EXPECT_RBRACE,
    ERR_EXPECT_EXPR,
    ERR_UNEXPECTED_TOKEN
} ErrorCode;

const char *error_msgs[] = {
    "No error",
    "Invalid character",
    "Expected ';'",
    "Expected ')'",
    "Expected '('",
    "Expected '}'",
    "Expected expression",
    "Unexpected token"
};

Token* peek();

bool get_error(){
	if (error_code != ERR_NONE) {
	    printf("[Error] %s at line %d before '%s' token\n", error_msgs[error_code], error_line, peek()->text);
	    return true;
	}
	return false;
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

Token* peek() { return &tokens[tok_pos]; }
Token* next() { return &tokens[tok_pos++]; }

bool match(TokenType t) {
    if (peek()->type == t) {
        tok_pos++;
        return true;
    }/*else if(t != TOK_NEXTLINE){
    	if(match(TOK_NEXTLINE))
    		return false;
	}*/
    
    return false;
}

bool expect(TokenType t, int err) {
    if (match(t))
        return true;

    if (error_code == ERR_NONE) {
        error_code = err;
        //--tok_pos;
        //while(match(TOK_NEXTLINE)) tok_pos -= 2;
        error_line = peek()->line;
    }

    return false;
}


void add_token(TokenType type, int value, const char* text, int line) {
    tokens[tok_count].type = type;
    tokens[tok_count].value = value;
    tokens[tok_count].line = line;
    if (text) strcpy(tokens[tok_count].text, text);
    tok_count++;
}

void lexer(char *src) {

    char *p = src;
    tok_count = 0;
    int line = 1;

    while (1) {
		if(*p == '\n'){
			//add_token(TOK_NEXTLINE, 0, NULL, line++);
			line++;
			p++;
		}
		
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

            add_token(TOK_NUM, 0, buf, line);
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

            if (!strcmp(buf,"if")) add_token(TOK_IF,0,NULL,line);
            else if (!strcmp(buf,"else")) add_token(TOK_ELSE,0,NULL,line);
            else if (!strcmp(buf,"while")) add_token(TOK_WHILE,0,NULL,line);
            else if (!strcmp(buf, "break")) add_token(TOK_BREAK,0,NULL,line);
			else if (!strcmp(buf, "continue")) add_token(TOK_CONTINUE,0,NULL,line);
			else if (!strcmp(buf, "byte")) add_token(TOK_BYTE,0,NULL,line);
			else if (!strcmp(buf, "word")) add_token(TOK_WORD,0,NULL,line);

            else add_token(TOK_IDENT,0,buf,line);

            continue;
        }

        // ---------------------------
        // operadores multi-char
        // ---------------------------
        if (p[0]=='=' && p[1]=='='){ add_token(TOK_EQUAL_EQUAL,0,NULL,line); p+=2; continue; }
        if (p[0]=='!' && p[1]=='='){ add_token(TOK_NOT_EQUAL,0,NULL,line); p+=2; continue; }
        if (p[0]=='<' && p[1]=='='){ add_token(TOK_LESS_EQ,0,NULL,line); p+=2; continue; }
        if (p[0]=='>' && p[1]=='='){ add_token(TOK_GREATER_EQ,0,NULL,line); p+=2; continue; }
        if (p[0]=='&' && p[1]=='&'){ add_token(TOK_AND,0,NULL,line); p+=2; continue; }
        if (p[0]=='|' && p[1]=='|'){ add_token(TOK_OR,0,NULL,line); p+=2; continue; }
        if (p[0]=='<' && p[1]=='<'){ add_token(TOK_SHL,0,NULL,line); p+=2; continue; }
        if (p[0]=='>' && p[1]=='>'){ add_token(TOK_SHR,0,NULL,line); p+=2; continue; }
        if (p[0]=='^' && p[1]=='^'){ add_token(TOK_EXP,0,NULL,line); p+=2; continue; }

        // ---------------------------
        // single-char
        // ---------------------------
        switch(*p){
            case '+': add_token(TOK_PLUS,0,NULL,line); break;
            case '-': add_token(TOK_MINUS,0,NULL,line); break;
            case '*': add_token(TOK_MUL,0,NULL,line); break;
            case '/': add_token(TOK_DIV,0,NULL,line); break;
            case '%': add_token(TOK_MOD,0,NULL,line); break;

            case '&': add_token(TOK_AND_BIT,0,NULL,line); break;
            case '|': add_token(TOK_OR_BIT,0,NULL,line); break;
            case '^': add_token(TOK_XOR_BIT,0,NULL,line); break;
            case '~': add_token(TOK_NOT_BIT,0,NULL,line); break;
            case '!': add_token(TOK_NOT,0,NULL,line); break;

            case '<': add_token(TOK_LESS,0,NULL,line); break;
            case '>': add_token(TOK_GREATER,0,NULL,line); break;

            case '=': add_token(TOK_ASSIGN,0,NULL,line); break;

            case '(': add_token(TOK_LPAREN,0,NULL,line); break;
            case ')': add_token(TOK_RPAREN,0,NULL,line); break;
            case ';': add_token(TOK_SEMI,0,NULL,line); break;
            case '{': add_token(TOK_LBRACE,0,NULL,line); break;
			case '}': add_token(TOK_RBRACE,0,NULL,line); break;
			default:
					// caractere inválido
				error_code = ERR_LEX_INVALID_CHAR;
				error_line = line;
				return;
        }
        p++;
    }

    add_token(TOK_EOF,0,NULL,line);
}


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


AST *parse_primary() {
    Token *t = peek();

    if (match(TOK_LPAREN)) {
        AST *n = parse_expression();
        match(TOK_RPAREN);
        return n;
    }
    
    /*
    if (match(TOK_IDENT))
        return new_ident(strdup(t->text));
    */
    
    if (match(TOK_IDENT)) {
	    if (find_symbol(t->text) == -1) {
	        if (error_code == ERR_NONE) {
	            error_code = ERR_UNEXPECTED_TOKEN;
	            error_line = t->line;
	        }
	        return NULL;
	    }
	    return new_ident(strdup(t->text));
	}

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

AST *parse_expression() {
    return parse_assign();
}

Stmt* parse_block() {
    Stmt *head = NULL;
    Stmt **curr = &head;

    while (peek()->type != TOK_RBRACE &&
           peek()->type != TOK_EOF)
    {
        *curr = parse_statement();
        if(*curr == NULL)	return NULL;
        curr = &((*curr)->next);
    }

    if(!expect(TOK_RBRACE, ERR_EXPECT_RBRACE))	return NULL;
    return head;
}

Stmt* parse_if() {
    expect(TOK_IF, ERR_UNEXPECTED_TOKEN);
    if (!expect(TOK_LPAREN, ERR_EXPECT_LPAREN))	return NULL;
    
	AST *cond = parse_expression();
	if (!cond) {
		if (error_code == ERR_NONE) {
        	error_code = ERR_EXPECT_EXPR;
        	error_line = peek()->line;
    	}
    	return NULL;
	}
    
	if (!expect(TOK_RPAREN, ERR_EXPECT_RPAREN))	return NULL;

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
    expect(TOK_WHILE, ERR_UNEXPECTED_TOKEN);
    if(!expect(TOK_LPAREN, ERR_EXPECT_LPAREN)) return NULL;
    
    AST *cond = parse_expression();
    
    if(!expect(TOK_RPAREN, ERR_EXPECT_RPAREN)) return NULL;

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_WHILE;
    s->expr = cond;
    s->body = parse_statement();
    
    s->next = NULL;
    return s;
}

Stmt* parse_break() {
    expect(TOK_BREAK, ERR_UNEXPECTED_TOKEN);
    if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) return NULL;

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_BREAK;
    s->expr = NULL;
    s->next = NULL;
    return s;
}

Stmt* parse_continue() {
    expect(TOK_CONTINUE, ERR_UNEXPECTED_TOKEN);
    if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) return NULL;

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_CONTINUE;
    s->expr = NULL;
    s->next = NULL;
    return s;
}

Stmt* parse_declaration() {
    VarType type;

    if (match(TOK_BYTE))
        type = TYPE_BYTE;
    else if (match(TOK_WORD))
        type = TYPE_WORD;
    else
        return NULL; // segurança

    // identificador obrigatório
    Token *t = peek();
    if (!expect(TOK_IDENT, ERR_UNEXPECTED_TOKEN))
        return NULL;

    char *name = strdup(t->text);

    // adiciona na tabela de símbolos
    if(!add_symbol(name, type))	
		return NULL;

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_DECL;
    s->ident = name;
    s->vtype = type;
    s->expr = NULL;
    s->next = NULL;

    // inicialização opcional
    if (match(TOK_ASSIGN)) {
        s->expr = parse_expression();
        if (!s->expr) {
            error_code = ERR_EXPECT_EXPR;
            error_line = peek()->line;
            return NULL;
        }
    }

    if (!expect(TOK_SEMI, ERR_EXPECT_SEMI))
        return NULL;

    return s;
}



Stmt* parse_expr_stmt() {
    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_EXPR;
    s->expr = parse_expression();
    if(!s->expr) return NULL;
    s->next = NULL;
    
	if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) return NULL;	
    
    return s;
}

Stmt* parse_statement() {
	if (peek()->type == TOK_LBRACE) {
        match(TOK_LBRACE);
        return parse_block();
    }
    
    if (peek()->type == TOK_BYTE || peek()->type == TOK_WORD)
        return parse_declaration();
    
    if (peek()->type == TOK_IF)
        return parse_if();

    if (peek()->type == TOK_WHILE)
        return parse_while();
        
    if (peek()->type == TOK_BREAK)
        return parse_break();

    if (peek()->type == TOK_CONTINUE)
        return parse_continue();

    return parse_expr_stmt();
}


void gen_math(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
	printf(" LD R%d\n", rx);
	if(type != NODE_NOT_BIT){
		gen(node->left, state, ++rx);
		rx--;
	}
    printf(" %s R%d\n", math_operation[type], rx);
}

void gen_math_exp(AST *node, bool* state, int rx){
	rx++;
	printf(" STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
    printf(" IDC\n");
    		
	gen(node->right, state, rx);
	printf(" LD R%d\n", rx++);
	printf(" DECR\n");
	printf(" JC @+6\n");
	printf(" STD 1\n");
	printf(" JP exp_end_%d\n", label_count);
	printf(" DECR\n");
    gen(node->left, state, rx);
    printf(" JC @+4\n");
    printf(" JP exp_end_%d\n", label_count);
    printf(" LD R%d\n", rx);
    printf(" %s R%d\n", math_operation[NODE_MUL], rx);
    printf(" DECR\n");
    printf(" JC @-2\n");
    printf("exp_end_%d:\n", label_count++);
}

void gen_move(AST *node, int bit, OperandType type, OperandType reg){
	if(type == LITERAL){
		(node->ident) ?
			printf(" STD %s::%d\n", node->ident, bit) 	:
			printf(" STD 0x%03X::%d\n", node->value & 0xFFF, bit);	
	}else{
		printf(" STL R%d\n", reg);
	}
}

void gen_shift(AST *node, bool* state, int rx, int type){
	if(node->right->type == NODE_NUM){
    	gen(node->left, state, rx);
    	if(node->right->value != 0)
    		printf(" %s %d\n", math_operation[type], node->right->value);
	}else{
		printf(" STD 0x%02X\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
	    printf(" IDC\n");
	    		
		gen(node->right, state, rx);
    	printf(" LD R%d\n", rx++);
    	printf(" DECR\n");
    	gen(node->left, state, rx);
    	printf(" %s 1\n", math_operation[type]);
    	printf(" DECR\n");
    	printf(" JC @-2\n");
	}
}

void gen_logic(AST *node, bool* state, int rx, int type){
	gen(node->right, state, rx);
	if(type != NODE_NOT){
		printf(" JZ @+4\n");
    	printf(" %s\n", cond_state[TRUE_INDEX]);
    	printf(" LD R%d\n", rx++);
    	gen(node->left, state, rx);
    	printf(" JZ @+4\n");
    	printf(" %s\n", cond_state[TRUE_INDEX]);
    	printf(" %s R%d\n", math_operation[type], --rx);	
	}else{
		printf(" JZ @+5\n");
		printf(" %s\n", cond_state[FALSE_INDEX]);
		printf(" JP @+4\n");
		printf(" %s\n", cond_state[TRUE_INDEX]);
		printf(" LD R%d\n", rx);
	}
}

void gen_addr(AST *node){
	gen_move(node, HIGH_PART, LITERAL, 0);
	printf(" OUT P0\n");
    gen_move(node, LOW_PART, LITERAL, 0);
    printf(" OUT P1\n");
}

bool is_assigning = false;
void gen_io_write(AST *node, bool* state, int rx){
	gen(node->right, state, rx);
	if(node->left->value > 0xFFF){
		printf(" OUT P%d\n", (node->left->value & 0x7));
	}else{
		printf(" PUSHD\n");
	    is_assigning = true;
		gen(node->left, state, rx);
		is_assigning = false;
	    printf(" POPD\n");
	    printf(" OUT P2\n");	
	} 
}

void gen_io_read(AST *node){	
	gen_addr(node);
	printf(" IN P2\n");
}

void gen_io_pointer(AST *node, bool* state, int rx, int number){
	static int depth = 0, depth_a = 0;
	
	bool increment = !(depth ^ is_assigning);
	bool idc_config = depth == 1 && increment || depth == 0 && node->right->type == NODE_IDENT;
	depth++;
	if(idc_config){
		printf(" STD 0x01\n");
    	printf(" IDC\n");
	}
			
	depth_a = depth;
	gen(node->right, state, rx);
			
	if(depth_a != depth){
		printf(" OUT P0\n");
		printf(" POPD\n");
		printf(" OUT P1\n");
	}else{
		if(node->right->type == NODE_NUM){
			printf(" OUT P1\n");
			gen_move(node->right, HIGH_PART, LITERAL, 0);
			printf(" OUT P0\n");	
		}else if(node->right->type == NODE_IDENT){
			printf(" PUSHD\n");
	    	printf(" INCR\n");
	    	printf(" IN P2\n");
	    	printf(" OUT P0\n");
			printf(" POPD\n");
			printf(" OUT P1\n");
		}
	}
    		
    if(increment)
		printf(" IN P2\n");
				
	if(--depth_a > 0){
		printf(" PUSHD\n");
	    printf(" INCR\n");
	    printf(" IN P2\n");
	}else{
		depth = depth_a;
	}	
}

void gen_branch_eqdiff(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	printf(" JZ @+%d\n", (type == NODE_EQUAL) ? off_true : off_true + 1);
    printf(" %s\n", (type == NODE_EQUAL) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    printf(" JP @+%d\n", (type == NODE_EQUAL) ? off_false : off_false - 1);
    printf(" %s\n", (type == NODE_EQUAL) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_geqlt(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	printf(" JC @+%d\n", (type == NODE_GREAT_EQ) ? off_true : off_true + 1);
    printf(" %s\n", (type == NODE_GREAT_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    printf(" JP @+%d\n", (type == NODE_GREAT_EQ) ? off_false : off_false - 1);
    printf(" %s\n", (type == NODE_GREAT_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_leqgt(int type, const char* state[]){
	int off_true_1 = 6;
	int off_true_2 = 4;
	int off_false = 5;
	
	printf(" JC @+%d\n", (type == NODE_LESS_EQ) ? off_true_1 : off_true_1 - 1);
    printf(" %s\n", (type == NODE_LESS_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
    printf(" JP @+%d\n", (type == NODE_LESS_EQ) ? off_false : off_false + 1);
    printf(" JZ @-%d\n", (type == NODE_LESS_EQ) ? off_true_2 : off_true_2 - 1);
    printf(" %s\n", (type == NODE_LESS_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
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
	
	while(s) {
	    switch(s->type) {
		    case STMT_EXPR: {
		        gen(s->expr, &st, 0);
		        break;
		    }
		
			case STMT_IF: {
			    int lbl_else = label_count++;
			    int lbl_end  = s->else_branch ? label_count++ : lbl_else;
			
			    gen(s->expr, &st, 0);
			
			    if (s->else_branch) {
			        printf(" JZ else_%d\n", lbl_else);
			        gen_stmt(s->then_branch);
			        printf(" JP endif_%d\n", lbl_end);
			        printf("else_%d:\n", lbl_else);
			        gen_stmt(s->else_branch);
			        printf("endif_%d:\n", lbl_end);
			    } else {
			        printf(" JZ endif_%d\n", lbl_else);
			        gen_stmt(s->then_branch);
			        printf("endif_%d:\n", lbl_else);
			    }
			    break;
			}

		    case STMT_WHILE: {
			    int lbl_begin = label_count++;
			    int lbl_end   = label_count++;
			
			    // salvar contexto anterior (para loops aninhados)
			    int old_begin = loop_begin_label;
			    int old_end   = loop_end_label;
			
			    loop_begin_label = lbl_begin;
			    loop_end_label   = lbl_end;
			
			    printf("while_begin_%d:\n", lbl_begin);
			
			    gen(s->expr, &st, 0);
			    printf(" JZ while_end_%d\n", lbl_end);
			
			    if (s->body)
			        gen_stmt(s->body);
			
			    printf(" JP while_begin_%d\n", lbl_begin);
			    printf("while_end_%d:\n", lbl_end);
			
			    // restaurar contexto
			    loop_begin_label = old_begin;
			    loop_end_label   = old_end;
			
			    break;
			}

			
			case STMT_BREAK:
			    printf(" JP while_end_%d\n", loop_end_label);
			    break;
			
			case STMT_CONTINUE:
			    printf(" JP while_begin_%d\n", loop_begin_label);
			    break;

	        case STMT_DECL: {
			    // reserva variável (Assembly label)
			    if (s->vtype == TYPE_BYTE)
			        printf("%s: DB 0\n", s->ident);
			    else
			        printf("%s: DW 0\n", s->ident);
			
			    // inicialização
			    if (s->expr) {
			        AST assign_node;
			        assign_node.type = NODE_ASSIGN;
			        assign_node.left = new_ident(s->ident);
			        assign_node.right = s->expr;
			        gen(&assign_node, &st, 0);
			    }
			    break;
			}

	    }
	
	    s = s->next;
	}
    return 0;
}

void compile(char *source) {
    
    error_code = ERR_NONE;
	error_line = 0;

	lexer(source);
    tok_pos = 0;
    
    if(get_error()) return;

    Stmt *head = NULL;
    Stmt **curr = &head;

    while (peek()->type != TOK_EOF) {
        *curr = parse_statement();
        if(*curr == NULL){
    		if(get_error()) 
				return;    	
		}
        curr = &((*curr)->next);
    }

    int result = gen_stmt(head);
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
