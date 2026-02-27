#ifndef _ASTLIB_H_
#define _ASTLIB_H_

#include <stdarg.h>
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

#include "assembler.h"

#define MAX_TOKENS 	8192
#define LOW_PART	0
#define HIGH_PART	8

#define NODE_COMP		NODE_EQUAL
#define FALSE_INDEX		0
#define TRUE_INDEX		1 


int label_count = 0;
int tok_count = 0;
int tok_pos = 0;
int stack_i = 0;
int declcount = 0;

int loop_begin_label = -1;
int loop_end_label = -1;

int error_code = 0;
int error_line = 0;
const char *error_msgs[];

char *code_buf = NULL;
size_t code_len = 0;

char *data_buf = NULL;
size_t data_len = 0;

char *func_buf = NULL;
size_t func_len = 0;

bool func_decl = false;
bool has_ssp = false;

char* final_buf = NULL;
char* function = NULL;

bool word_decl = false;
bool word_attr = false;

typedef enum {
	PARSER,
	GENERATOR
} StepType;

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
    TOK_LPAREN, TOK_RPAREN,	// ( )
    TOK_LBRACE,	TOK_RBRACE,	// { }

    TOK_SEMI,				// ;
    TOK_COMMA,				// ,

    // keywords
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_BREAK,
	TOK_CONTINUE,
	TOK_BYTE,
	TOK_WORD,
	TOK_QUOTE,
	TOK_STRING,
	TOK_RETURN
} TokenType;


typedef struct {
    TokenType type;
    int value;
    char text[64];
    int line;
} Token;

Token* peek();
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
    NODE_POINTER,
    NODE_CALL,
    NODE_STRING,
    NODE_ADDRESS,
    NODE_NEG
} NodeType;

typedef struct AST {
    NodeType type;
    int value;          // usado se NODE_NUM
    char *ident;        // usado se NODE_IDENT
    struct AST *left;
    struct AST *right;
    
    // novo
    struct AST **args;
    int arg_count;
} AST;

typedef enum {
    TYPE_BYTE = 1,
    TYPE_WORD = 2
} VarType;

typedef enum {
    GLOBAL,
    LOCAL,
    PARAM
} ScopeType;

typedef struct {
    char *name;
    int addr;
    VarType type;
    ScopeType scope;
} Symbol;

typedef struct Scope {
	Symbol *var;
	int vars;
	int childs;
	int type;
	int allocs;
	struct Scope **child;
	struct Scope *parent;
} Scope;

Scope *global_scope = NULL;
Scope *current_scope = NULL;
Scope *scope_var = NULL;

typedef enum {
    STMT_EXPR,
    STMT_IF,
    STMT_WHILE,
    STMT_BREAK,
	STMT_CONTINUE,
	STMT_DECL,
	STMT_FUNCTION,
	STMT_RETURN
} StmtType;

typedef struct Stmt {
    StmtType type;
    AST *expr;
    struct Stmt *then_branch;
    struct Stmt *else_branch;
    struct Stmt *body;
    struct Stmt *next;
    
	char *func_name;
    struct Stmt *func_body;
    
    // --- NOVO ---
    char *ident;
    VarType vtype;
} Stmt;


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
int eval(AST*, bool*);
int gen(AST*, bool, int);
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
    ERR_EXPECT_LBRACE,
    ERR_EXPECT_RBRACE,
    ERR_EXPECT_EXPR,
    ERR_UNDECLARED_VARIABLE,
    ERR_EXPECT_STMT,
    ERR_UNEXPECTED_TOKEN
} ErrorCode;

const char *error_msgs[] = {
    "No error",
    "Invalid character",
    "Expected ';'",
    "Expected ')'",
    "Expected '('",
    "Expected '{",
    "Expected '}'",
    "Expected expression",
    "Undeclared variable",
    "Expected statement",
    "Unexpected token",
};

// Criação de funções
// -----------------------------------------------------------------------
typedef struct {
    char name[32];
    VarType ret_type;
    int param_count;
} Function;

#define MAX_FUNCTIONS 64
Function functab[MAX_FUNCTIONS];
int funccount = 0;

int find_function(const char *name) {
    for (int i = 0; i < funccount; i++)
        if (strcmp(functab[i].name, name) == 0)
            return i;
    return -1;
}

bool add_function(const char *name, VarType type, int param_count) {
    if (find_function(name) != -1) {
        printf("[error] function '%s' already exists\n", name);
        return false;
    }

    strcpy(functab[funccount].name, name);
    functab[funccount].ret_type = type;
    functab[funccount].param_count = param_count;
    funccount++;
    return true;
}
// -----------------------------------------------------------------------

Scope *create_scope(){
	Scope *scope = calloc(1, sizeof(Scope));
	/*
	scope->var = NULL;
	scope->parent = NULL;
	scope->child = NULL;
	scope->childs = 0;
	scope->type = GLOBAL;
	scope->allocs = 0;
	scope->vars = 0;
	*/
	return scope;
}

void enter_scope(StepType type){
	Scope *parent = current_scope;
	int i = parent->childs;
	if(type == PARSER){
		parent->child = realloc(parent->child, (i + 1) * sizeof(Scope*));
		parent->child[i] = calloc(1, sizeof(Scope));
		parent->child[i]->parent = parent;
	}
	parent->childs++;
	current_scope = parent->child[i];
	current_scope->type = LOCAL;
	current_scope->allocs = 0;
}

void leave_scope(){
	current_scope->childs = 0;
	current_scope = current_scope->parent;
}

int find_local_var(const char *name) {
    for (int i = 0; i < current_scope->vars; i++) {
        if (strcmp(current_scope->var[i].name, name) == 0)
            return i;
    }
    return -1;
}

int find_vars(const char *name){
	scope_var = current_scope;
	while(scope_var){
		for (int i = 0; i < scope_var->vars; i++) {
	        if (strcmp(scope_var->var[i].name, name) == 0)
	            return i;
    	}
    	scope_var = scope_var->parent;
	}
    return -1;
}

bool add_var(char *name, VarType type, ScopeType scope, int addr) {
	int var_i = find_local_var(name);
    if (var_i != -1) {
    	if(error_code == ERR_NONE){
    		error_code = ERR_UNEXPECTED_TOKEN;
    		error_line = peek()->line;
		}
    	printf("[error] the variable '%s' already exists!\r\n", name);
        return false;
    }
	
	int i = current_scope->vars;
	current_scope->var = realloc(current_scope->var, (i + 1) * sizeof(Symbol));
	
	current_scope->var[i].name = name;
    current_scope->var[i].type = type;
    current_scope->var[i].scope = scope;
    current_scope->var[i].addr = addr;
    current_scope->vars++;
    
    return true;
}

bool get_error(){
	if (error_code != ERR_NONE) {
		--tok_pos;
	    printf("[Error] %s at line %d after '%s' token\r\n", error_msgs[error_code], error_line, peek()->text);
	    return true;
	}
	return false;
}

#define EMIT_FUNC(...) emit(&func_buf, &func_len, __VA_ARGS__)
#define EMIT_CODE(...) (!func_decl) ? emit(&code_buf, &code_len, __VA_ARGS__) : EMIT_FUNC(__VA_ARGS__)
#define EMIT_DATA(...) emit(&data_buf, &data_len, __VA_ARGS__)


void emit(char **buf, size_t *len, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    // 1) Descobrir tamanho necessário
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (needed <= 0) {
        va_end(args);
        return;
    }

    // 2) Realocar buffer
    char *new_buf = realloc(*buf, *len + needed + 1);
    if (!new_buf) {
        va_end(args);
        return; // ou tratar erro
    }

    *buf = new_buf;

    // 3) Escrever no final
    vsnprintf(*buf + *len, needed + 1, fmt, args);

    *len += needed;

    va_end(args);
}

void skip_spaces(char **p, int *line) {
    while (**p) {
        // espaços
        if (isspace(**p)) {
        	if(**p == '\n') (*line)++;
			(*p)++;
			continue; 
		}

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
    }
    
    return false;
}

bool expect(TokenType t, int err) {
    if (match(t))
        return true;

    if (error_code == ERR_NONE) {
        error_code = err;
        --tok_pos;
        error_line = peek()->line;
        ++tok_pos;
    }

    return false;
}


void add_token(TokenType type, int value, const char* text, int line) {
    tokens[tok_count].type = type;
    tokens[tok_count].value = value;
    tokens[tok_count].line = line;
    if (text) 
		strcpy(tokens[tok_count].text, text);
	else
		strcpy(tokens[tok_count].text, "EOF");
    tok_count++;
}

void wrx_lexer(char *src) {

    char *p = src;
    char buf[64];
    tok_count = 0;
    int line = 1;

    while (1) {
		if(*p == '\n'){
			line++;
			p++;
		}
		
        skip_spaces(&p, &line);
        if (*p == 0) break;

		// -------------------------------------
		// Strings
		// ------------------------------------
		if(*p == '"'){
			buf[0] = *p++; buf[1] = 0;
			add_token(TOK_QUOTE,0,buf,line);
			
			int i = 0;
			while(*p != '"' && *p != '\0')
				buf[i++] = *p++;
			buf[i] = 0;
			if(*p == '\0'){
				if(error_code == ERR_NONE){
					error_code = ERR_LEX_INVALID_CHAR;
					error_line = line;
				}
				return;
			}
			add_token(TOK_STRING,0,buf,line);
			
			buf[0] = *p++; buf[1] = 0;
			add_token(TOK_QUOTE,0,buf,line);
			continue;
		}
        // ---------------------------
        // números (texto bruto)
        // suporta: $FF, 0xFF, FFh, H'FF', 'A'
        // ---------------------------
        if (isdigit(*p) || is_hexa(p) || *p == '\'') {
            int i = 0;
			bool is_quote = (*p == '\'');
            while (*p && !isspace(*p) && !strchr("+-*/%&|^~!=<>();,", *p) || is_quote)
            {
                buf[i++] = *p++;
				if(is_quote && *p == '\''){
					buf[i++] = *p++;
					is_quote = false;
				}
            }
            buf[i] = 0;

            add_token(TOK_NUM, 0, buf, line);
            continue;
        }

        // ---------------------------
        // identificador / keyword
        // ---------------------------
        if (is_alpha(*p)) {
            int i = 0;

            while (is_alnum(*p))
                buf[i++] = *p++;
            buf[i] = 0;

            if (!strcmp(buf,"if")) 				add_token(TOK_IF, 0, buf, line);
            else if (!strcmp(buf,"else")) 		add_token(TOK_ELSE, 0, buf, line);
            else if (!strcmp(buf,"while")) 		add_token(TOK_WHILE, 0, buf, line);
            else if (!strcmp(buf, "break")) 	add_token(TOK_BREAK, 0, buf, line);
			else if (!strcmp(buf, "continue")) 	add_token(TOK_CONTINUE, 0, buf, line);
			else if (!strcmp(buf, "byte")) 		add_token(TOK_BYTE, 0, buf, line);
			else if (!strcmp(buf, "word")) 		add_token(TOK_WORD, 0, buf, line);
			else if (!strcmp(buf, "return")) 	add_token(TOK_RETURN, 0, buf, line);

            else add_token(TOK_IDENT,0,buf,line);

            continue;
        }

		buf[0] = *p; buf[1] = *(p+1); buf[2] = 0;
		
        // ---------------------------
        // operadores multi-char
        // ---------------------------
        if (p[0]=='=' && p[1]=='='){ add_token(TOK_EQUAL_EQUAL,0,buf,line); p+=2; continue; }
        if (p[0]=='!' && p[1]=='='){ add_token(TOK_NOT_EQUAL,0,buf,line); p+=2; continue; }
        if (p[0]=='<' && p[1]=='='){ add_token(TOK_LESS_EQ,0,buf,line); p+=2; continue; }
        if (p[0]=='>' && p[1]=='='){ add_token(TOK_GREATER_EQ,0,buf,line); p+=2; continue; }
        if (p[0]=='&' && p[1]=='&'){ add_token(TOK_AND,0,buf,line); p+=2; continue; }
        if (p[0]=='|' && p[1]=='|'){ add_token(TOK_OR,0,buf,line); p+=2; continue; }
        if (p[0]=='<' && p[1]=='<'){ add_token(TOK_SHL,0,buf,line); p+=2; continue; }
        if (p[0]=='>' && p[1]=='>'){ add_token(TOK_SHR,0,buf,line); p+=2; continue; }
        if (p[0]=='^' && p[1]=='^'){ add_token(TOK_EXP,0,buf,line); p+=2; continue; }

		buf[0] = *p; buf[1] = 0;
        // ---------------------------
        // single-char
        // ---------------------------
        switch(*p){
            case '+': add_token(TOK_PLUS,0,buf,line); break;
            case '-': add_token(TOK_MINUS,0,buf,line); break;
            case '*': add_token(TOK_MUL,0,buf,line); break;
            case '/': add_token(TOK_DIV,0,buf,line); break;
            case '%': add_token(TOK_MOD,0,buf,line); break;

            case '&': add_token(TOK_AND_BIT,0,buf,line); break;
            case '|': add_token(TOK_OR_BIT,0,buf,line); break;
            case '^': add_token(TOK_XOR_BIT,0,buf,line); break;
            case '~': add_token(TOK_NOT_BIT,0,buf,line); break;
            case '!': add_token(TOK_NOT,0,buf,line); break;

            case '<': add_token(TOK_LESS,0,buf,line); break;
            case '>': add_token(TOK_GREATER,0,buf,line); break;

            case '=': add_token(TOK_ASSIGN,0,buf,line); break;

            case '(': add_token(TOK_LPAREN,0,buf,line); break;
            case ')': add_token(TOK_RPAREN,0,buf,line); break;
            case ';': add_token(TOK_SEMI,0,buf,line); break;
            case '{': add_token(TOK_LBRACE,0,buf,line); break;
			case '}': add_token(TOK_RBRACE,0,buf,line); break;
			case ',': add_token(TOK_COMMA,0,buf,line); break;
			default: {
				if(error_code == ERR_NONE){
					error_code = ERR_LEX_INVALID_CHAR;
					error_line = line;
				}
				return;
			}
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
	if(!name) return NULL;
    AST *n = calloc(1, sizeof(AST));
    n->type = NODE_IDENT;
    n->value = 0;
    n->ident = name;
    n->left = n->right = NULL;
    return n;
}

AST *new_string(char *value) {
	if(!value) return NULL;
    AST *n = calloc(1, sizeof(AST));
    n->type = NODE_STRING;
    n->value = 0;
    n->ident = value;
    n->left = n->right = NULL;
    return n;
}

AST *new_op(NodeType type, AST *l, AST *r) {
	if(!r) return NULL;
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

    if(tokens[tok_pos - 1].type != TOK_RPAREN){
    	if (match(TOK_LPAREN)) {
	        AST *n = parse_expression();
	        match(TOK_RPAREN);
	        return n;
    	}
	}
	
	bool num_addr = (t->type == TOK_NUM && tokens[tok_pos + 1].type == TOK_LPAREN) ||
					(t->type == TOK_LPAREN && tokens[tok_pos - 1].type == TOK_RPAREN);
					
	if (match(TOK_IDENT) || num_addr) {
	    char *name = strdup(t->text);
	    
	    bool is_num = false;
	    if(t->type == TOK_NUM)
	    	is_num = match(TOK_NUM);
		
	    // chamada?
	    if (match(TOK_LPAREN)) {
	        AST *call = calloc(1, sizeof(AST));
	        call->type = NODE_CALL;
	        call->ident = name;
	        call->value = is_num;
	
	        call->args = NULL;
	        call->arg_count = 0;
	
	        // argumentos
	        if (!match(TOK_RPAREN)) {
	            while (1) {
	                AST *arg = parse_expression();
	                if (!arg) return NULL;
					
	                call->args = realloc(call->args,
	                    sizeof(AST*) * (call->arg_count + 1));
	
	                call->args[call->arg_count++] = arg;
					
	                if (match(TOK_RPAREN))
	                    break;
	
	                if(!expect(TOK_COMMA, ERR_UNEXPECTED_TOKEN)) return NULL;
	            }
	        }
	
	        return call;
	    }
	
	    // variável normal
	    if (find_vars(name) == -1) {
	        if(find_function(name) == -1){
	        	error_code = ERR_UNDECLARED_VARIABLE;
		        error_line = t->line;
		        --tok_pos;
		        return NULL;
			}
	    }
	
	    return new_ident(name);
	}

    if (match(TOK_NUM)){
    	t->value = parse_number(t->text);
    	return new_num(t->value);
	}
	
	if(match(TOK_QUOTE)){
		char* str = strdup(peek()->text);
		match(TOK_STRING);
		match(TOK_QUOTE);
		return new_string(str);
	}
        
    return NULL;
}

AST *parse_unary() {
	if (match(TOK_MUL))
        return new_op(NODE_POINTER, NULL, parse_unary());
        
    if (match(TOK_AND_BIT))
        return new_op(NODE_ADDRESS, NULL, parse_unary());
        
    if (match(TOK_NOT_BIT))
        return new_op(NODE_NOT_BIT, NULL, parse_unary());

    if (match(TOK_NOT))
        return new_op(NODE_NOT, NULL, parse_unary());
    
	if (match(TOK_MINUS))
        return new_op(NODE_NEG, NULL, parse_unary());

    return parse_primary();
}

AST *parse_mul() {
    AST *node = parse_unary();
    if(!node) return NULL;

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
	if(!node) return NULL;
	
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
	if(!node) return NULL;
	
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
	if(!node) return NULL;
	
    while (match(TOK_AND))
        node = new_op(NODE_AND, node, parse_relational());

    return node;
}

AST *parse_logical_or() {
    AST *node = parse_logical_and();
    if(!node) return NULL;

    while (match(TOK_OR))
        node = new_op(NODE_OR, node, parse_logical_and());

    return node;
}

AST *parse_assign() {
    AST *node = parse_logical_or();
    if(!node) return NULL;

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

	enter_scope(PARSER);
    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_IF;
    s->expr = cond;
    s->then_branch = parse_statement();
    if(!s->then_branch){
    	if(error_code == ERR_NONE){
    		error_code = ERR_EXPECT_STMT;
    		error_line = peek()->line;
    		--tok_pos;
		}
    	return NULL;
	}
	leave_scope();

    if (match(TOK_ELSE)){
    	enter_scope(PARSER);
    	 s->else_branch = parse_statement();
    	 if(!s->else_branch){
	    	if(error_code == ERR_NONE){
	    		error_code = ERR_EXPECT_STMT;
	    		error_line = peek()->line;
			}
	    	return NULL;
		}
		leave_scope();
	}
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

	enter_scope(PARSER);
    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_WHILE;
    s->expr = cond;
    s->body = parse_statement();
    if(!s->body)
    	if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) return NULL;
    leave_scope();
    
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

Stmt* parse_return() {
    expect(TOK_RETURN, ERR_UNEXPECTED_TOKEN);

    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_RETURN;
    s->expr = parse_expression();
    s->next = NULL;
    
    if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) 
		return NULL;
    return s;
}

Stmt* parse_declaration() {
    VarType type;
    ScopeType scope = current_scope->type;

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
	stack_i = (scope != GLOBAL) ? stack_i + type : 0;
	
    // adiciona na tabela de símbolos
    if(!add_var(name, type, scope, stack_i))	
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
        	if(error_code == ERR_NONE){
        		error_code = ERR_EXPECT_EXPR;
            	error_line = peek()->line;	
			}
            
            return NULL;
        }
    }
    
	if(peek()->type != TOK_LPAREN)
    	if (!expect(TOK_SEMI, ERR_EXPECT_SEMI))
        	return NULL;

    return s;
}

Stmt* parse_function() {

    VarType ret;

    if (match(TOK_BYTE)) ret = TYPE_BYTE;
    else match(TOK_WORD), ret = TYPE_WORD;

    Token *t = peek();
    expect(TOK_IDENT, ERR_UNEXPECTED_TOKEN);
    char *name = strdup(t->text);

    expect(TOK_LPAREN, ERR_EXPECT_LPAREN);

	stack_i = 0;
    int param_count = 0;
	enter_scope(PARSER);
    if (!match(TOK_RPAREN)) {
    	int param_i = 4;
        while (1) {

            VarType ptype;
            if (match(TOK_BYTE)) ptype = TYPE_BYTE;
            else if (match(TOK_WORD)) ptype = TYPE_WORD;
            else return NULL;

            Token *pt = peek();
            expect(TOK_IDENT, ERR_UNEXPECTED_TOKEN);
			
			char* varname = strdup(pt->text);
            if(!add_var(varname, ptype, PARAM, param_i)) return NULL;
            param_count++;
            param_i += ptype;

            if (match(TOK_RPAREN))
                break;

            expect(TOK_COMMA, ERR_UNEXPECTED_TOKEN);
        }
    }
	
	
    add_function(name, ret, param_count);

    //expect(TOK_LBRACE, ERR_EXPECT_LBRACE);
    
	func_decl = true;
	Stmt *s = calloc(1, sizeof(Stmt));
	s->type = STMT_FUNCTION;
	s->func_name = name;
	s->func_body = parse_statement();
	func_decl = false;
	leave_scope();
	return s;
}



Stmt* parse_expr_stmt() {
    Stmt *s = calloc(1, sizeof(Stmt));
    s->type = STMT_EXPR;
    s->expr = parse_expression();
    if (!s->expr) return NULL;
    s->next = NULL;
    
    if(peek()->type != TOK_LPAREN)
		if(!expect(TOK_SEMI, ERR_EXPECT_SEMI)) 
			return NULL;
    
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
        
    if(peek()->type == TOK_RETURN)
    	return parse_return();

    return parse_expr_stmt();
}

int optimizer(AST *expr, bool is_assign){
	bool st = true;
	bool isnull = (expr->left) ? !expr->left->ident : false;
	int result = eval(expr, &st);
	if(st && isnull)
		EMIT_CODE(" STD 0x%03X\r\n", result);
	else
		return gen(expr, is_assign, 0);
	return 1;
}

bool is_address = false;

void operate_high_part(int rx, int type){
	bool is_add = type == NODE_ADD || type == NODE_SHT_LEFT;
	bool is_sub = type == NODE_SUB || type == NODE_SHT_RIGHT;
	
	if((is_add || is_sub) && word_decl){
		if(is_add) { 
			EMIT_CODE(" JC @+12\r\n");
			EMIT_CODE(" LD R%d\r\n", rx);
			EMIT_CODE(" POPD\r\n");
			if(type == NODE_ADD || type == NODE_SUB){
	            EMIT_CODE(" POP R%d\r\n", ++rx);
	    		EMIT_CODE(" %s R%d\r\n", math_operation[type], rx--);        
        	}else if(type == NODE_SHT_LEFT){
        		EMIT_CODE(" SHL 1\r\n");
			}else{
				EMIT_CODE(" SHR 1\r\n");
			}
        	EMIT_CODE(" PUSHD\r\n");
        	EMIT_CODE(" STL R%d\r\n", rx);
			EMIT_CODE(" JP @+12\r\n"); 
		}else{
			EMIT_CODE(" JC @+12\r\n"); 
		}
		
		EMIT_CODE(" LD R%d\r\n", rx);
		EMIT_CODE(" STD 0x80\r\n");
		EMIT_CODE(" IDC\r\n");
		EMIT_CODE(" POPD\r\n");
		
		(is_add) ? EMIT_CODE(" INCR\r\n") : EMIT_CODE(" DECR\r\n");
		
		if(type == NODE_ADD || type == NODE_SUB){
            EMIT_CODE(" POP R%d\r\n", ++rx);
    		EMIT_CODE(" %s R%d\r\n", math_operation[type], rx--);        
        }
		
		EMIT_CODE(" PUSHD\r\n");
		EMIT_CODE(" STL R%d\r\n", rx);	
	}
}

void gen_math(AST *node, bool is_assign, int rx, int type){
	//optimizer(node->right);	-> Next level optimization
	gen(node->right, is_assign, rx);
	EMIT_CODE(" LD R%d\r\n", rx);
	if(type != NODE_NOT_BIT){
		gen(node->left, is_assign, ++rx);
		rx--;
	}
    EMIT_CODE(" %s R%d\r\n", math_operation[type], rx);
    operate_high_part(rx, type);
}

void gen_math_exp(AST *node, bool is_assign, int rx){
	rx++;
	EMIT_CODE(" STD 0x%02X\r\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
    EMIT_CODE(" IDC\r\n");
    
	gen(node->right, is_assign, rx);
	EMIT_CODE(" LD R%d\r\n", rx++);
	EMIT_CODE(" DECR\r\n");
	EMIT_CODE(" JC @+6\r\n");
	EMIT_CODE(" STD 1\r\n");
	EMIT_CODE(" JP exp_end_%d\r\n", label_count);
	EMIT_CODE(" DECR\r\n");
    gen(node->left, is_assign, rx);
    EMIT_CODE(" JC @+4\r\n");
    EMIT_CODE(" JP exp_end_%d\r\n", label_count);
    EMIT_CODE(" LD R%d\r\n", rx);
    EMIT_CODE(" %s R%d\r\n", math_operation[NODE_MUL], rx);
    EMIT_CODE(" DECR\r\n");
    EMIT_CODE(" JC @-2\r\n");
    EMIT_CODE("exp_end_%d:\r\n", label_count++);
}

void gen_move(AST *node, int bit, OperandType type, OperandType reg){
	if(type == LITERAL){
		if(node->ident)
			EMIT_CODE(" STD %s::%d\r\n", node->ident, bit);
		else if(!bit){
			if(node->value > 0xFF || word_decl){
				EMIT_CODE(" STD 0x%03X::8\r\n", node->value & 0xFFFF);
				EMIT_CODE(" PUSHD\r\n");
				word_attr = true;
			}
			EMIT_CODE(" STD 0x%03X\r\n", node->value & 0xFFFF);
		}
		else{
			EMIT_CODE(" STD 0x%03X::%d\r\n", node->value & 0xFFF, bit);	
		}		
	}else{
		EMIT_CODE(" STL R%d\r\n", reg);
	}
}

void gen_shift(AST *node, bool is_assign, int rx, int type){
	static int i = 0;
	if(node->right->type == NODE_NUM){
    	gen(node->left, is_assign, rx);
    	if(node->right->value != 0){
    		EMIT_CODE(" %s %d\r\n", math_operation[type], node->right->value);
			operate_high_part(rx, type);	
		}
	}else{
		EMIT_CODE(" STD 0x%02X\r\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx & 0x07));
	    EMIT_CODE(" IDC\r\n");
	    
	    int index = i++;
		gen(node->left, is_assign, rx);
		EMIT_CODE(" PUSHD\r\n");		
		gen(node->right, is_assign, rx);
		EMIT_CODE(" JZ skip_shift_%d\r\n", index);
    	EMIT_CODE(" LD R%d\r\n", rx++);
    	EMIT_CODE(" DECR\r\n");
    	EMIT_CODE(" POPD\r\n");
    	EMIT_CODE(" %s 1\r\n", math_operation[type]);
    	EMIT_CODE(" DECR\r\n");
    	EMIT_CODE(" JC @-2\r\n");
    	EMIT_CODE(" PUSHD\r\n");
    	EMIT_CODE(" skip_shift_%d:\r\n", index);
    	EMIT_CODE(" POPD\r\n");
	}
}

void gen_logic(AST *node, bool is_assign, int rx, int type){
	gen(node->right, is_assign, rx);
	if(type != NODE_NOT){
		EMIT_CODE(" JZ @+4\r\n");
    	EMIT_CODE(" %s\r\n", cond_state[TRUE_INDEX]);
    	EMIT_CODE(" LD R%d\r\n", rx++);
    	gen(node->left, is_assign, rx);
    	EMIT_CODE(" JZ @+4\r\n");
    	EMIT_CODE(" %s\r\n", cond_state[TRUE_INDEX]);
    	EMIT_CODE(" %s R%d\r\n", math_operation[type], --rx);	
	}else{
		EMIT_CODE(" JZ @+5\r\n");
		EMIT_CODE(" %s\r\n", cond_state[FALSE_INDEX]);
		EMIT_CODE(" JP @+4\r\n");
		EMIT_CODE(" %s\r\n", cond_state[TRUE_INDEX]);
		EMIT_CODE(" LD R%d\r\n", rx);
	}
}

void gen_addr(AST *node){
	gen_move(node, HIGH_PART, LITERAL, 0);
	EMIT_CODE(" OUT P0\r\n");
    gen_move(node, LOW_PART, LITERAL, 0);
    EMIT_CODE(" OUT P1\r\n");
}

// Aloca um byte local
void alloc_local_byte(){
	EMIT_CODE(" STD 1\r\n");
	EMIT_CODE(" SSP\r\n");
}

// Aloca uma word local
void alloc_local_word(){
	EMIT_CODE(" STD 2\r\n");
	EMIT_CODE(" SSP\r\n");
}

// Ler endereço nomeado
void read_address_ident(AST *node){
	EMIT_CODE(" STD %s::8\r\n", node->ident);
	EMIT_CODE(" PUSHD\r\n");
	EMIT_CODE(" STD %s::0\r\n", node->ident);
}

// Endereço Nomeado
void write_address_ident(AST *node){
	EMIT_CODE(" STD %s::8\r\n", node->ident);
	EMIT_CODE(" OUT P0\r\n");
	EMIT_CODE(" STD %s::0\r\n", node->ident);
	EMIT_CODE(" OUT P1\r\n");
}

// Endereço Numérico
void write_address_number(AST *node){
	EMIT_CODE(" STD 0x%03X::8\r\n", node->value);
	EMIT_CODE(" OUT P0\r\n");
	EMIT_CODE(" STD 0x%03X::0\r\n", node->value);
	EMIT_CODE(" OUT P1\r\n");
}

// Escreve endereço
void read_address(){
    EMIT_CODE(" IN P0\r\n");
    EMIT_CODE(" PUSHD\r\n");
    EMIT_CODE(" IN P1\r\n");
    EMIT_CODE(" PUSHD\r\n");	
}

// Escreve endereço
void write_address(){
    EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" OUT P1\r\n");
    EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" OUT P0\r\n");	
}

// Escreve endereço Sem 1ª POP
void write_address_opt(){
    EMIT_CODE(" OUT P1\r\n");
    EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" OUT P0\r\n");	
}

// Ler byte global
void read_global_byte(){
	EMIT_CODE(" IN P2\r\n");
}

// Ler word global
void read_global_word(){
	EMIT_CODE(" STD 0x01\r\n");
	EMIT_CODE(" IDC\r\n");
	EMIT_CODE(" INCR\r\n");
	EMIT_CODE(" IN P2\r\n");
	EMIT_CODE(" PUSHD\r\n");
	EMIT_CODE(" DECR\r\n");
	EMIT_CODE(" IN P2\r\n");
}

// Escreve byte global
void write_global_byte(){
	EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" OUT P2\r\n");	
}

// Escreve word global
void write_global_word(){
	EMIT_CODE(" STD 0x01\r\n");
	EMIT_CODE(" IDC\r\n");
	EMIT_CODE(" POPD\r\n");
	EMIT_CODE(" OUT P2\r\n");
	EMIT_CODE(" INCR\r\n");
	EMIT_CODE(" POPD\r\n");
	EMIT_CODE(" OUT P2\r\n");
}

// Ler byte local
void read_local_byte(int offset){
    EMIT_CODE(" STD %d\r\n", offset);
    EMIT_CODE(" SBP\r\n");
}

// Ler word local
void read_local_word(int offset){
    EMIT_CODE(" STD %d\r\n", offset - 1);
    EMIT_CODE(" SBP\r\n");
    EMIT_CODE(" PUSHD\r\n");
    EMIT_CODE(" STD %d\r\n", offset);
    EMIT_CODE(" SBP\r\n");
}

// Escreve byte local
void write_local_byte(int offset){
	EMIT_CODE(" LD R2\r\n");
	EMIT_CODE(" STD %d\r\n", offset);
	EMIT_CODE(" SBW\r\n");
}

// Escreve word local
void write_local_word(int offset){
    EMIT_CODE(" LD R2\r\n");
    EMIT_CODE(" STD %d\r\n", offset);
    EMIT_CODE(" SBW\r\n");
    
    EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" LD R2\r\n");
    EMIT_CODE(" STD %d\r\n", offset - 1);
    EMIT_CODE(" SBW\r\n");	
}


// Ler parâmetro byte
void read_param_byte(int offset){
	EMIT_CODE(" STD %d\r\n", offset);
	EMIT_CODE(" ABP\r\n");
}

// Ler parâmetro word
void read_param_word(int offset){
	EMIT_CODE(" STD %d\r\n", offset + 1);
	EMIT_CODE(" ABP\r\n");
	EMIT_CODE(" PUSHD\r\n");
	EMIT_CODE(" STD %d\r\n", offset);
	EMIT_CODE(" ABP\r\n");
}

// Escreve parâmetro byte
void write_param_byte(int offset){
	EMIT_CODE(" LD R2\r\n");
	EMIT_CODE(" STD %d\r\n", offset);
	EMIT_CODE(" SBW\r\n");
}

// Escreve parâmetro word
void write_param_word(int offset){
    EMIT_CODE(" LD R2\r\n");
    EMIT_CODE(" STD %d\r\n", offset);
    EMIT_CODE(" SBW\r\n");
    
    EMIT_CODE(" POPD\r\n");
    EMIT_CODE(" LD R2\r\n");
    EMIT_CODE(" STD %d\r\n", offset - 1);
    EMIT_CODE(" SBW\r\n");	
}

// Escreve byte local na atribuição
void write_local_byte_assign(){
	EMIT_CODE(" POP R0\r\n");
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" STL R0\r\n");
	EMIT_CODE(" SBW\r\n");
}

// Escreve word local na atribuição
void write_local_word_assign(){
	EMIT_CODE(" STD 0x80\r\n");
	EMIT_CODE(" IDC\r\n");
	EMIT_CODE(" POP R0\r\n");
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" STL R0\r\n");
	EMIT_CODE(" SBW\r\n");
	
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" DECR\r\n");
	EMIT_CODE(" SBW\r\n");
}

// Escreve parâmetro byte
void write_param_byte_assign(){
	EMIT_CODE(" STD 0x80\r\n");
	EMIT_CODE(" IDC\r\n");
	EMIT_CODE(" POP R0\r\n");
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" NOT R0\r\n");
	EMIT_CODE(" INCR\r\n");
	EMIT_CODE(" SBW\r\n");
}

// Escreve parâmetro word na atribuição
void write_param_word_assign(){
	EMIT_CODE(" STD 0x80\r\n");
	EMIT_CODE(" IDC\r\n");
	EMIT_CODE(" POP R0\r\n");
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" NOT R0\r\n");
	EMIT_CODE(" INCR\r\n");
	EMIT_CODE(" SBW\r\n");
	
	EMIT_CODE(" POP R2\r\n");
	EMIT_CODE(" DECR\r\n");
	EMIT_CODE(" SBW\r\n");
}


void read_local_address(int offset){
	EMIT_CODE(" STD %d\r\n", offset);
}

void read_local_pointer(){
	EMIT_CODE(" SBP\r\n");
}

void read_param_pointer(){
	EMIT_CODE(" ABP\r\n");
}

void save_lresult(){
	EMIT_CODE(" PUSHD\r\n");
}

int gen_io_write(AST *node, bool is_assign, int rx){
    bool isGlobal = false;
    bool isWord = false;
    bool isParam = false;
    int offset = 0;
		
	word_decl = false;
	word_attr = false;
	
	if(node->left->value > 0xFFF){
		optimizer(node->right, false);
		EMIT_CODE(" OUT P%d\r\n", (node->left->value & 0x7));
	}else{
		int var = -1;
		is_assign = node->left->ident != NULL;
        if(is_assign){
             var = find_vars(node->left->ident);
             if(var == -1){
                if(error_code == ERR_NONE){
        			error_code = ERR_UNEXPECTED_TOKEN;
        			error_line = peek()->line;
        		}
        		printf("Error: Undeclared variable '%s'!\n", node->left->ident);
        		return 0;          
            }        
        }else{
           	AST *expr = node->left;
           	while(expr->type != NODE_IDENT && expr->type != NODE_NUM){
           		if(expr->type == NODE_POINTER || expr->type == NODE_ADDRESS){
           			expr = expr->right;
					continue;	
				}
           		expr = expr->left;
			}
			if(expr->type == NODE_NUM){
           		isGlobal = true;
			}else{
           		int idx = find_vars(expr->ident);
           		if(idx == -1){
	                if(error_code == ERR_NONE){
	        			error_code = ERR_UNEXPECTED_TOKEN;
	        			error_line = peek()->line;
	        		}
	        		printf("Error: Undeclared variable '%s'!\n", expr->ident);
	        		return 0;          
            	}
            	isGlobal = scope_var->var[idx].scope == GLOBAL;
	            isParam = scope_var->var[idx].scope == PARAM;
	            isWord = scope_var->var[idx].type == TYPE_WORD;
	            word_decl = isWord;
	            offset = (isParam) ? -scope_var->var[idx].addr : scope_var->var[idx].addr;
			}
			
			optimizer(node->right, false);
	        save_lresult();
	        gen(node->left, true, rx);
        }
        
        if(var != -1){
        	// Se for identificador de variável
            isGlobal = scope_var->var[var].scope == GLOBAL;
            isParam = scope_var->var[var].scope == PARAM;
            isWord = scope_var->var[var].type == TYPE_WORD;
            word_decl = isWord;
            offset = (isParam) ? -scope_var->var[var].addr : scope_var->var[var].addr;
            
            if(is_assign)
            	optimizer(node->right, false);
            
            if(isGlobal){
            	save_lresult();
                 // Identificador Global
                 (is_assign)  ? write_address_ident(node->left)
                              : write_address();
                             
                 (isWord && word_attr)		? write_global_word()
                             				: write_global_byte();
            }else if(isParam){
                  // Identificador de Parâmetro de Função
                 (isWord && word_attr)		? write_param_word(offset)
                             				: write_param_byte(offset);
            }else{
                 // Identificador de Variável Local Interna
                 (isWord && word_attr)		? write_local_word(offset)
                             				: write_local_byte(offset);      
            }    
        }else{
        	// Se não for identificador (Pode ser expressão, número, etc)
			save_lresult();
			if(isGlobal){
            	write_address();
				(isWord && word_attr)		? write_global_word()
                             				: write_global_byte();
            }else if(isParam)
				(isWord && word_attr)		? write_param_word_assign()
							 				: write_param_byte_assign();
			else{
            	(isWord && word_attr)		? write_local_word_assign()
            				 				: write_local_byte_assign();
            }
        }
	}
	
	return 1;
}

int extra_arg = 0;


int gen_io_read(AST *node, bool is_assign){
    int var = -1;
    if(node->ident){
         var = find_vars(node->ident);
         if(var == -1){
            if(error_code == ERR_NONE){
    			error_code = ERR_UNEXPECTED_TOKEN;
    			error_line = peek()->line;
    		}
    		printf("Error: Undeclared variable '%s'!\n", node->ident);
    		return 0;          
        }           
    }
    
    bool isGlobal = false;
    bool isWord = false;
    bool isParam = false;
    bool isAddrNum = var == -1;
    int offset = 0;
  
    if(!isAddrNum){
        isGlobal = scope_var->var[var].scope == GLOBAL;
        isParam = scope_var->var[var].scope == PARAM;
        isWord = scope_var->var[var].type == TYPE_WORD;
        word_attr = isWord || word_attr;
        offset = scope_var->var[var].addr;
        
        if(isGlobal){
             // Identificador Global
             if(is_assign)
             	read_address_ident(node);
             else{
             	write_address_ident(node);
	        	(isWord && word_decl)		? read_global_word()
	                        				: read_global_byte();
			 }
			  
        }else if(isParam){
              // Identificador de Parâmetro de Função
        	if(is_assign)
            	read_local_address(offset);
            else{
            	(isWord && word_decl)		? read_param_word(offset)
                         					: read_param_byte(offset); 	
			}    
        }else{
             // Identificador de Variável Local Interna
        	if(is_assign)
            	read_local_address(offset);
            else{
            	(isWord && word_decl)		? read_local_word(offset)
                         					: read_local_byte(offset); 	
			}      
        }
    }else{
        // Endereço Numérico - Usado em Atribuições
        write_address_number(node);
    }
    
	return 1;
}

int gen_io_pointer(AST *node, bool is_assign, int rx){
     static bool isGlobal = false;
     static bool isWord = false;
     static bool isParam = false;
     static bool word_prev = false;
     static int count = 0;
     static int offset = 0;
     static int var = -1;
     
     if(node->right->ident){
        var = find_vars(node->right->ident);
        if(var == -1){
            if(error_code == ERR_NONE){
     			error_code = ERR_UNEXPECTED_TOKEN;
     			error_line = peek()->line;
      		}
      		printf("Error: Undeclared variable '%s'!\n", node->right->ident);
      		return 0;          
        }
        isGlobal = scope_var->var[var].scope == GLOBAL;
        isParam = scope_var->var[var].scope == PARAM;
        isWord = scope_var->var[var].type == TYPE_WORD;
        word_prev = word_decl;
		word_decl = isWord;
        offset = (isParam) ? -scope_var->var[var].addr : scope_var->var[var].addr;
        if(!isWord && isGlobal){
            if(error_code == ERR_NONE){
                error_code = ERR_UNEXPECTED_TOKEN;
  			    error_line = peek()->line;
   		    }
   		    printf("Error: Expected WORD, but '%s' is BYTE!\n", node->right->ident);
            return 0;   
        }
     }else if(node->right->type == NODE_NUM){
           isGlobal = true;
           isWord = true;
           word_prev = word_decl;
           word_decl = isWord;  
     }else if(node->right->type != NODE_POINTER){
           // TODO: Fazer busca de identificador/endereço base na expressão     
     }
     
     ++count;
     gen(node->right, is_assign, rx);
   	 --count;
   	 
   	 if(isGlobal || isWord){
   	 	//save_lresult();
         write_address_opt();
         (word_prev || count) 	? read_global_word()
         						: read_global_byte();
     }else if(isParam)
    	read_param_pointer();
     else{
		read_local_pointer();
     }
     return 1;
}

bool is_param = false;
int gen_io_address(AST *node, bool is_assign, int rx){
	char *address = node->right->ident;
	if(address){
		int var = find_vars(address);
		int func = find_function(address);
		int scope = (var != -1) ? scope_var->var[var].scope : GLOBAL;
		scope = (func != -1) ? GLOBAL : scope;
		
		if(scope == GLOBAL){
			EMIT_CODE(" STD %s::8\r\n", address);
			EMIT_CODE(" PUSHD\r\n");
			EMIT_CODE(" STD %s::0\r\n", address);
			if(is_param)	++extra_arg;
		}else{
			int addr = scope_var->var[var].addr;
			if(scope_var->var[var].scope == PARAM){
				if(scope_var->var[var].type == TYPE_WORD){
					EMIT_CODE(" STD %d\r\n", addr + 1);	// MOD HERE
					EMIT_CODE(" ABP\r\n");
					EMIT_CODE(" PUSHD\r\n");
					EMIT_CODE(" STD %d\r\n", addr);
					EMIT_CODE(" ABP\r\n");
				}else{
					EMIT_CODE(" STD %d\r\n", addr);
				}
			}
		}
		is_address = true;
	}else{
		gen(node->right, is_assign, rx);
	}
	return 1;
}

void gen_branch_eqdiff(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	EMIT_CODE(" JZ @+%d\r\n", (type == NODE_EQUAL) ? off_true : off_true + 1);
    EMIT_CODE(" %s\r\n", (type == NODE_EQUAL) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    EMIT_CODE(" JP @+%d\r\n", (type == NODE_EQUAL) ? off_false : off_false - 1);
    EMIT_CODE(" %s\r\n", (type == NODE_EQUAL) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_geqlt(int type, const char* state[]){
	int off_true = 5;
	int off_false = 4;
	
	EMIT_CODE(" JC @+%d\r\n", (type == NODE_GREAT_EQ) ? off_true : off_true + 1);
    EMIT_CODE(" %s\r\n", (type == NODE_GREAT_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
    EMIT_CODE(" JP @+%d\r\n", (type == NODE_GREAT_EQ) ? off_false : off_false - 1);
    EMIT_CODE(" %s\r\n", (type == NODE_GREAT_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
}

void gen_branch_leqgt(int type, const char* state[]){
	int off_true_1 = 6;
	int off_true_2 = 4;
	int off_false = 5;
	
	EMIT_CODE(" JC @+%d\r\n", (type == NODE_LESS_EQ) ? off_true_1 : off_true_1 - 1);
    EMIT_CODE(" %s\r\n", (type == NODE_LESS_EQ) ? state[TRUE_INDEX] : state[FALSE_INDEX]);
    EMIT_CODE(" JP @+%d\r\n", (type == NODE_LESS_EQ) ? off_false : off_false + 1);
    EMIT_CODE(" JZ @-%d\r\n", (type == NODE_LESS_EQ) ? off_true_2 : off_true_2 - 1);
    EMIT_CODE(" %s\r\n", (type == NODE_LESS_EQ) ? state[FALSE_INDEX] : state[TRUE_INDEX]);
}

void gen_relational(AST *node, bool is_assign, int rx, int type, const char* cond[]){
	gen_math(node, is_assign, rx, NODE_COMP);
	if(type == NODE_EQUAL || type == NODE_DIFF)
		gen_branch_eqdiff(type, cond);
	else if(type == NODE_GREAT_EQ || type == NODE_LESS)
		gen_branch_geqlt(type, cond);
	else{
		gen_branch_leqgt(type, cond);
	}	
}

int args = 0;

int gen_functions_call(AST *node, bool is_assign, int rx){
	bool is_paren_open = strcmp(node->ident, "(") == 0;
	if(!node->value && node->ident && !is_paren_open){
		int idx = find_function(node->ident);
		if (idx == -1) {
			idx = find_vars(node->ident);
			if(idx == -1){
			    printf("[error] function '%s' not declared\n", node->ident);
			    return 0;
			}
		}
	}
		
	if(is_paren_open){
		EMIT_CODE(" POP R%d\r\n", ++rx);
		EMIT_CODE(" POP R%d\r\n", ++rx);
		EMIT_CODE(" STD 0x%02X\r\n", (0b01 << 6) | ((rx & 0x07) << 3) | (rx - 1 & 0x07));
	    EMIT_CODE(" IDC\r\n");
		EMIT_CODE(" DECR\r\n");
	}
			
	// push argumentos (ordem inversa)
	for (int i = node->arg_count - 1; i >= 0; i--) {
		is_param = true;
		gen(node->args[i], is_assign, rx);
		EMIT_CODE(" PUSHD\r\n");
	}
	
	args += extra_arg;
	is_param = false;
			
	if(is_paren_open){
		EMIT_CODE(" STD (@+8) >> 8\r\n");
		EMIT_CODE(" PUSHD\r\n");
		EMIT_CODE(" STD (@+5) & 0xFF\r\n");
		EMIT_CODE(" PUSHD\r\n");
		    	
		EMIT_CODE(" PUSH R%d\r\n", rx--);
		EMIT_CODE(" PUSH R%d\r\n", rx--);
		EMIT_CODE(" RET\r\n");
	}else{
		EMIT_CODE(" CALL %s\r\n", node->ident);
	}
		
	if(node->arg_count){
		EMIT_CODE(" LD R%d\r\n", rx);
		EMIT_CODE(" STD %d\r\n SSP\r\n", -(node->arg_count + args));
		EMIT_CODE(" STL R%d\r\n", rx);
	}
	
	args = 0;
	extra_arg = 0;
	return 1;
}

int gen_string (AST* node){
	EMIT_CODE(" JP @+%d\r\n", 3+strlen(node->ident));
	EMIT_CODE(" DB \"%s\",0\r\n", node->ident);
	EMIT_CODE(" STD (@-%d) >> 8\r\n", strlen(node->ident)+1);	// MOD HERE
	EMIT_CODE(" PUSHD\r\n");
	EMIT_CODE(" STD (@-%d) & 0xFF\r\n", strlen(node->ident)+4);
	args++;
	return 1;	
}

int gen_negative(AST* node, bool is_assign, int rx){
	EMIT_CODE(" STD 1\r\n");
	EMIT_CODE(" LD R%d\r\n", rx);
	gen_math(node, is_assign, ++rx, NODE_NOT_BIT);
	EMIT_CODE(" %s R%d\r\n", math_operation[NODE_ADD], --rx);
	return 1;
}

int eval(AST *node, bool* state) {
	if(*state){
	    switch (node->type) {
	        case NODE_NUM: 	 	 return node->value;
	        case NODE_ADD: 	 	 return eval(node->left, state) + eval(node->right, state);
	        case NODE_SUB: 	 	 return eval(node->left, state) - eval(node->right, state);
	        case NODE_MUL: 	 	 return eval(node->left, state) * eval(node->right, state);
	        case NODE_DIV: 	 	 {
	        	int num1 = eval(node->left, state);
	        	int num2 = eval(node->right, state);
	        	return (!num1 || !num2) ? 0 : (num1 / num2);
			}
	        case NODE_OR: 	 	 return eval(node->left, state) || eval(node->right, state);
	        case NODE_AND: 	 	 return eval(node->left, state) && eval(node->right, state);
	        case NODE_EQUAL: 	 return eval(node->left, state) == eval(node->right, state);
	        case NODE_DIFF:  	 return eval(node->left, state) != eval(node->right, state);
	        case NODE_LESS:  	 return eval(node->left, state) < eval(node->right, state);
	        case NODE_GREAT:   	 return eval(node->left, state) > eval(node->right, state);
	        case NODE_LESS_EQ: 	 return eval(node->left, state) <= eval(node->right, state);
	        case NODE_GREAT_EQ:  return eval(node->left, state) >= eval(node->right, state);
	        case NODE_OR_BIT: 	 return eval(node->left, state) | eval(node->right, state);
	        case NODE_XOR_BIT: 	 return eval(node->left, state) ^ eval(node->right, state);
	        case NODE_MOD: 		 {
	        	int num1 = eval(node->left, state);
	        	int num2 = eval(node->right, state);
	        	return (!num1 || !num2) ? 0 : (num1 % num2);
			}
	        case NODE_SHT_LEFT:  return eval(node->left, state) << eval(node->right, state);
	        case NODE_SHT_RIGHT: return eval(node->left, state) >> eval(node->right, state);
	        case NODE_AND_BIT: 	 return eval(node->left, state) & eval(node->right, state);
	        case NODE_NOT_BIT: 	 return ~eval(node->right, state);
	        case NODE_NOT: 		 return !eval(node->right, state);
	        case NODE_NEG:		 return -eval(node->right, state);
	        case NODE_EXP: 		 return (int)pow(eval(node->left, state), eval(node->right, state));
	        case NODE_IDENT: 	{	*state = false;	return 0;	}
			case NODE_POINTER: 	{ 	*state = false;	return 0;	}
			case NODE_ADDRESS: 	{ 	*state = false;	return 0;	}
			case NODE_ASSIGN:  	{
				if(!node->left->ident)	{	*state = false;	return 0;	}
				return eval(node->right, state);
			}
			case NODE_CALL: 	{
				*state = false;	return 0;
				break;
			}
			case NODE_STRING:	{
				*state = false;	return 0;
				break;
			}
	    }		
	}

    return 0;
}

int gen(AST *node, bool is_assign, int rx) {
	if(!node) return 0;
	
    switch (node->type) {
        case NODE_NUM:		 {
        	gen_move(node, LOW_PART, LITERAL, 0);
			return 1;
		}
        case NODE_ADD:		 {
        	gen_math(node, is_assign, rx, NODE_ADD);
			return 2;
		}
        case NODE_SUB:		 {
        	gen_math(node, is_assign, rx, NODE_SUB);
			return 3;
		}
        case NODE_MUL:		 {
        	gen_math(node, is_assign, rx, NODE_MUL);
			return 1;
		}
        case NODE_DIV:		 {
        	EMIT_CODE(" PUSH R0\r\n");
        	gen_math(node, is_assign, rx, NODE_DIV);
        	EMIT_CODE(" POP R0\r\n");
			return 1;
		}
		case NODE_MOD: 		 {
			gen_math(node, is_assign, rx, NODE_DIV);
			gen_move(node, 0, REGISTER, R0);
			return 1;
		}
		case NODE_AND_BIT: 	 {
			gen_math(node, is_assign, rx, NODE_AND_BIT);
			return 1;
		}
		case NODE_OR_BIT: 	 {
			gen_math(node, is_assign, rx, NODE_OR_BIT);
			return 1;
		}
		case NODE_XOR_BIT: 	 {
			gen_math(node, is_assign, rx, NODE_XOR_BIT);
			return 1;
		}
		case NODE_NOT_BIT: 	 {
			gen_math(node, is_assign, rx, NODE_NOT_BIT);
			return 1;
		}
		case NODE_SHT_LEFT:  {
			gen_shift(node, is_assign, rx, NODE_SHT_LEFT);
			return 1;
		}
        case NODE_SHT_RIGHT: {
        	gen_shift(node, is_assign, rx, NODE_SHT_RIGHT);
			return 1;
		}
		case NODE_OR: 	 	 {
			gen_logic(node, is_assign, rx, NODE_OR_BIT);
			return 1;
		}
        case NODE_AND: 	 	 {
        	gen_logic(node, is_assign, rx, NODE_AND_BIT);
			return 1;
		}
		case NODE_NOT: 		 {
			gen_logic(node, is_assign, rx, NODE_NOT);
			return 1;
		}
		case NODE_EXP: 		 {
			gen_math_exp(node, is_assign, rx);
			return 1;
		}
        case NODE_EQUAL: 	 {
        	gen_relational(node, is_assign, rx, NODE_EQUAL, cond_state);
			return 1;
		}
        case NODE_DIFF:  	 {
        	gen_relational(node, is_assign, rx, NODE_DIFF, cond_state);
			return 1;
		}
        case NODE_LESS:  	 {
        	gen_relational(node, is_assign, rx, NODE_LESS, cond_state);
			return 1;
		}
        case NODE_GREAT:   	 {
        	gen_relational(node, is_assign, rx, NODE_GREAT, cond_state);
			return 1;
		}
        case NODE_LESS_EQ: 	 {
        	gen_relational(node, is_assign, rx, NODE_LESS_EQ, cond_state);
			return 1;
		}
        case NODE_GREAT_EQ:  {
        	gen_relational(node, is_assign, rx, NODE_GREAT_EQ, cond_state);
			return 1;
		}
        case NODE_ASSIGN: 	 {
        	return gen_io_write(node, false, rx);
		}
        case NODE_IDENT:	 {
        	return gen_io_read(node, is_assign);
		}
		case NODE_POINTER:	 {
			return gen_io_pointer(node, is_assign, rx);
		}
		case NODE_ADDRESS:	 {
			return gen_io_address(node, is_assign, rx);
		}
		case NODE_CALL: {
		    return gen_functions_call(node, is_assign, rx);
		}
		case NODE_STRING:	{
			return gen_string(node);
		}
		case NODE_NEG:		{
			return gen_negative(node, is_assign, rx);
		}

    }
    return 1;
}

int gen_stmt(Stmt *s) {
	if(!s) return 0;
	
	bool st=true;
	
	while(s) {
	    switch(s->type) {
		    case STMT_EXPR: {
		    	// Optimization Point
				// ----------------------------------------------------- 
				if(!optimizer(s->expr, false))	return 0;
				// -----------------------------------------------------
		        //gen(s->expr, false, 0);
		        is_address = false;
		        break;
		    }
		
			case STMT_IF: {
			    int lbl_else = label_count++;
			    int lbl_end  = s->else_branch ? label_count++ : lbl_else;
			
				// Optimization Point
				// ----------------------------------------------------- 
				if(!optimizer(s->expr, false))	return 0;
				// -----------------------------------------------------
			    //gen(s->expr, &st, 0);
			
			    if (s->else_branch) {
			        EMIT_CODE(" JZ else_%d\r\n", lbl_else);
			        enter_scope(GENERATOR);
			        if(!gen_stmt(s->then_branch)) return 0;
			        if(current_scope->allocs){
		    			EMIT_CODE(" STD %d\r\n SSP\r\n", -current_scope->allocs);
		    			current_scope->allocs = 0;
					}
			        leave_scope();
			        EMIT_CODE(" JP endif_%d\r\n", lbl_end);
			        EMIT_CODE("else_%d:\r\n", lbl_else);
			        enter_scope(GENERATOR);
			        if(!gen_stmt(s->else_branch)) return 0;
			        if(current_scope->allocs){
		    			EMIT_CODE(" STD %d\r\n SSP\r\n", -current_scope->allocs);
		    			current_scope->allocs = 0;
					}
			        leave_scope();
			        EMIT_CODE("endif_%d:\r\n", lbl_end);
			    } else {
			        EMIT_CODE(" JZ endif_%d\r\n", lbl_else);
			        enter_scope(GENERATOR);
			        if(!gen_stmt(s->then_branch)) return 0;
			        if(current_scope->allocs){
		    			EMIT_CODE(" STD %d\r\n SSP\r\n", -current_scope->allocs);
		    			current_scope->allocs = 0;
					}
			        leave_scope();
			        EMIT_CODE("endif_%d:\r\n", lbl_else);
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
			
			    EMIT_CODE("while_begin_%d:\r\n", lbl_begin);
			
				// Optimization Point
				// ----------------------------------------------------- 
				if(s->expr){
					if(!optimizer(s->expr, false))	return 0;
					
					EMIT_CODE(" JZ while_end_%d\r\n", lbl_end);
				}
				// -----------------------------------------------------
			    //gen(s->expr, &st, 0);
			
				enter_scope(GENERATOR);
			    if (s->body)
			        if(!gen_stmt(s->body)) return 0;
			    if(current_scope->allocs){
		    		EMIT_CODE(" STD %d\r\n SSP\r\n", -current_scope->allocs);
		    		current_scope->allocs = 0;
				}
			    leave_scope();
			
			    EMIT_CODE(" JP while_begin_%d\r\n", lbl_begin);
			    EMIT_CODE("while_end_%d:\r\n", lbl_end);
			
			    // restaurar contexto
			    loop_begin_label = old_begin;
			    loop_end_label   = old_end;
			
			    break;
			}

			
			case STMT_BREAK:
			    EMIT_CODE(" JP while_end_%d\r\n", loop_end_label);
			    break;
			
			case STMT_CONTINUE:
			    EMIT_CODE(" JP while_begin_%d\r\n", loop_begin_label);
			    break;

	        case STMT_DECL: {
	        	
				// Optimization Point
				// -----------------------------------------------------
				
				int eval_result = 0;
	        	st = true;
				int var_index = find_vars(s->ident);
				
	        	if(var_index != -1){
	        		if(scope_var->var[var_index].scope == GLOBAL){
	        			if(s->expr)
							eval_result = eval(s->expr, &st);
						if(!st)	eval_result = 0;		
					}
				}
				// -----------------------------------------------------
					
				NodeType type = (s->expr) ? s->expr->type : NODE_NUM;
				if(scope_var->var[var_index].scope == GLOBAL){
					if(type != NODE_STRING){
						char* vtype = (s->vtype == TYPE_BYTE) ? "DB" : "DW";
					    EMIT_DATA("%s:\r\n %s %d\r\n", s->ident, vtype, eval_result);
					}else{
						char* vtype = (s->vtype == TYPE_BYTE) ? "DB" : "DW";
					    EMIT_DATA("%s:\r\n %s \"%s\",0\r\n", s->ident, vtype, s->expr->ident);
					}
				}else if (scope_var->var[var_index].scope == LOCAL){
					has_ssp = true;
					if(type != NODE_STRING){
						EMIT_CODE(" STD %d\r\n SSP\r\n", s->vtype);
						current_scope->allocs += s->vtype;
					}else{
						int size_str = strlen(s->expr->ident);
						EMIT_CODE(" STD %d\r\n SSP\r\n", size_str+1);
						int i = 0;
						for(; i < size_str; i++){
							EMIT_CODE(" STD '%c'\r\n LD R2\r\n", s->expr->ident[i]);
							EMIT_CODE(" STD %d\r\n SBW\r\n", (i+1));
						}
						EMIT_CODE(" CDR\r\n LD R2\r\n");
						EMIT_CODE(" STD %d\r\n SBW\r\n", (i+1));
						current_scope->allocs += (size_str+1);
					}
				}
			
			    // inicialização
			    if(type != NODE_STRING){
			    	if (s->expr && !st || s->expr && scope_var->var[var_index].scope == LOCAL) {
				        AST assign_node;
				        assign_node.type = NODE_ASSIGN;
				        assign_node.left = new_ident(s->ident);
				        assign_node.right = s->expr;
				        if(!gen(&assign_node, false, 0)) return 0;
			    	}
				}
				
				is_address = false;
			    break;
			}
			
			case STMT_FUNCTION: {
				func_decl = true;
				function = s->func_name;
				EMIT_CODE("\r\n%s:\r\n", function);
				EMIT_CODE(" PUSHB\r\n PUSHS\r\n POPB\r\n\r\n");
    			
    			enter_scope(GENERATOR);
				if(!gen_stmt(s->func_body)) return 0;
    			
    			EMIT_CODE("\r\n__%s_end:\r\n", function);
				if(has_ssp) 
					EMIT_CODE("\r\n PUSHB\r\n POPS");
    			EMIT_CODE("\r\n POPB\r\n");
    			EMIT_CODE(" RET\r\n");
    			
				leave_scope();
    			function = NULL;
    			func_decl = false;
				break;
			}
			
			case STMT_RETURN: {
				
				if(s->expr){
					int result = eval(s->expr, &st);
					
					if(st && function){
						int idx = find_function(function);
						if(functab[idx].ret_type == TYPE_WORD){
							EMIT_CODE(" STD %d::8\r\n", result);
							EMIT_CODE(" LD R0\r\n");
						}
						if(result > 255)
							EMIT_CODE(" STD %d::0\r\n", result);
						else
							EMIT_CODE(" STD %d\r\n", result);
					}else{
						if(!gen(s->expr, false, 0)) return 0;	
					}
				}
				
				if(function)		
					EMIT_CODE(" JP __%s_end\r\n", function);
				else{
					EMIT_CODE("\r\n POPB\r\n");
    				EMIT_CODE(" RET\r\n");
				}
				break;
			}

	    }
	
	    s = s->next;
	}
    return 1;
}


void wrx_parser(Stmt **head){
    Stmt **curr = head;

	global_scope = create_scope();
	current_scope = global_scope;
	
    while (peek()->type != TOK_EOF) {

        // ---- Função global?
        if ((peek()->type == TOK_BYTE || peek()->type == TOK_WORD) &&
            tokens[tok_pos + 1].type == TOK_IDENT &&
            tokens[tok_pos + 2].type == TOK_LPAREN)
        {
            *curr = parse_function();
        }
        else
        {
        	
            *curr = parse_statement();
        }

        if (*curr == NULL)
            return;

        curr = &((*curr)->next);
    }

    current_scope->childs = 0;
}


int wrx_builder(Stmt* parsing){
	return gen_stmt(parsing);
}

void append_buffer(char **dest, const char *src)
{
    if (!src) return;

    size_t dest_len = (*dest) ? strlen(*dest) : 0;
    size_t src_len  = strlen(src);

    char *new_buf = realloc(*dest, dest_len + src_len + 1);
    if (!new_buf) {
        fprintf(stderr, "Erro de memória\r\n");
        exit(1);
    }

    memcpy(new_buf + dest_len, src, src_len + 1); // copia com '\0'
    *dest = new_buf;
}

void build_buffer(void)
{
    final_buf = NULL;

    append_buffer(&final_buf, " JP __main\r\n\r\n");
    append_buffer(&final_buf, data_buf);

    append_buffer(&final_buf, "\r\n__main:\r\n");
    append_buffer(&final_buf, " PUSHB\r\n PUSHS\r\n POPB\r\n\r\n");
    append_buffer(&final_buf, code_buf);
    //append_buffer(&final_buf, " ED\r\n");	// <- temporario (debug)
    append_buffer(&final_buf, " JP __end\r\n");
    
    append_buffer(&final_buf, func_buf);
    append_buffer(&final_buf, "\r\n__end:\r\n");
}


typedef enum {
	_DATA,
	_CODE,
	_FUNC,
	_FULL
}SectionType;

char* compile(char *source) {

	unsigned char *mach = NULL;
	Stmt *syntax = NULL;    
    error_code = ERR_NONE;
	error_line = 0;

	wrx_lexer(source);
    if(get_error()) return NULL;

    wrx_parser(&syntax);
	if(get_error()) return NULL;
	
    if(!wrx_builder(syntax)) return NULL;
    if(get_error()) return NULL;
    
    build_buffer();
    if(!assemble_buffer(final_buf, &mach, false))
    	mach = NULL;
    
    return (char*)mach;
}

void show_asm(SectionType section){
	switch(section){
		case _DATA: printf("%s\r\n", data_buf ? data_buf : "");
					break;
		case _CODE:	printf("%s\r\n", code_buf ? code_buf : "");
					break;
		case _FUNC:	printf("%s\r\n", func_buf ? func_buf : "");
					break;
		case _FULL:	printf("%s\r\n", final_buf ? final_buf : "");
					break;
	}
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
