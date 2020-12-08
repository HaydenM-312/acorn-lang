#pragma once
#include "main.h"
#include <ctype.h>

enum Token_Types {
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_TIMES,
	TOKEN_DIV,
	TOKEN_EQU,
	TOKEN_GTE,
	TOKEN_NULLJOINER,
	TOKEN_GTEQU,
	TOKEN_MOD,
	TOKEN_LTE,
	TOKEN_LTEQU,
	TOKEN_NEQ,
	TOKEN_RANGE,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_NAME,
	TOKEN_INTEGER,
	TOKEN_FLOAT,
	TOKEN_STRING,
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELIF,
	TOKEN_ELSE,
	TOKEN_LET,
	TOKEN_SET,
	TOKEN_NIL,
	TOKEN_CASE,
	TOKEN_MATCH,
	TOKEN_ARROW,
	TOKEN_EOF,
	TOKEN_SOF
};
char* Token_Types_Print[] = {
	"TOKEN_AND",
	"TOKEN_OR",
	"TOKEN_PLUS",
	"TOKEN_MINUS",
	"TOKEN_TIMES",
	"TOKEN_DIV",
	"TOKEN_EQU",
	"TOKEN_GTE",
	"TOKEN_NULLJOINER",
	"TOKEN_GTEQU",
	"TOKEN_MOD",
	"TOKEN_LTE",
	"TOKEN_LTEQU",
	"TOKEN_NEQ",
	"TOKEN_RANGE",
	"TOKEN_LPAREN",
	"TOKEN_RPAREN",
	"TOKEN_LBRACKET",
	"TOKEN_RBRACKET",
	"TOKEN_NAME",
	"TOKEN_INTEGER",
	"TOKEN_FLOAT",
	"TOKEN_STRING",
	"TOKEN_IF",
	"TOKEN_THEN",
	"TOKEN_ELIF",
	"TOKEN_ELSE",
	"TOKEN_LET",
	"TOKEN_SET",
	"TOKEN_NIL",
	"TOKEN_CASE",
	"TOKEN_MATCH",
	"TOKEN_ARROW",
	"TOKEN_EOF",
	"TOKEN_SOF"
};

typedef struct TokenArr {
	Token* arr;
	long size;
} TokenArray;

struct Tokens {
	int type;
	long line;
	char* value;
};

Token new_token(int new_type, long new_line, char new_value[]);

// TokenArray functions
void init_array(TokenArray *array) {
	array->arr = (Token*)malloc(sizeof(Token));
	array->arr[0] = new_token(TOKEN_SOF, 0L, '\0');
	array->size = 1;
}

void append_array(TokenArray *array, Token tok) {
	array->size++;
	array->arr = (Token*)realloc(array->arr, sizeof(Token) * array->size);
	(array->arr)[array->size-1] = tok;
}

void free_array(TokenArray *array) {
	free(array->arr);
}

// Token functions
void print_token(Token t) {
	printf("[type:%s, line:%d", Token_Types_Print[t.type], t.line);
	if (t.value != NULL) {
		printf(", val:%s]\n", t.value);
	} else {
		printf(", val:NULL]\n");
	}
}

void init_token(Token *t, int new_type, long new_line, char new_value[]) {
	t->type = new_type;
	t->line = new_line;
	if (new_value != NULL) {
		t->value = malloc(strlen(new_value));
		t->value = new_value;
	} else {
		t->value = NULL;
	}
}

Token new_token(int new_type, long new_line, char new_value[]) {
	Token t;
	t.type = new_type;
	t.line = new_line;
	if (new_value != NULL) {
		t.value = malloc(strlen(new_value) + 1);
		t.value = new_value;
	} else {
		t.value = NULL;
	}
	return t;
}

void free_token(Token *t) {
	free(t->value);
}

Token* tokenize(char text[]) {
	TokenArray tokens;
	long line = 1;
	init_array(&tokens);
	for(size_t i = 0; text[i] != '\0'; i++) {
		switch (text[i]) {
		// Misc
		case '\n': // Increment line variable on newline, for better error handling
			line++;
			break;
		case ';':
			while(text[i+1] != '\n') i++;
		case '.':
			if (text[i+1] == '.') {append_array(&tokens, new_token(TOKEN_RANGE, line, '\0')); i++;}
			break;
		case '|':
			if (text[i+1] == '>') {append_array(&tokens, new_token(TOKEN_NULLJOINER, line, '\0')); i++;}
			break;
		// Basic arithmatic
		case '+':
			append_array(&tokens, new_token(TOKEN_PLUS, line, '\0'));
			break;
		case '-':
			if (text[i+1] == '>') {append_array(&tokens, new_token(TOKEN_ARROW, line, '\0')); i++;}
			else append_array(&tokens, new_token(TOKEN_MINUS, line, '\0'));
			break;
		case '*':
			append_array(&tokens, new_token(TOKEN_TIMES, line, '\0'));
			break;
		case '%':
			append_array(&tokens, new_token(TOKEN_MOD, line, '\0'));
			break;
		case '/':
			if (text[i+1] == '=') {append_array(&tokens, new_token(TOKEN_NEQ, line, '\0')); i++;}
			else append_array(&tokens, new_token(TOKEN_DIV, line, '\0'));
			break;
		// Comparison operators
		case '=':
			append_array(&tokens, new_token(TOKEN_EQU, line, '\0'));
			break;
		case '<':
			if (text[i+1] == '=') { append_array(&tokens, new_token(TOKEN_LTEQU, line, '\0')); i++;}
			else append_array(&tokens, new_token(TOKEN_LTE, line, '\0'));
			break;
		case '>':
			if (text[i+1] == '=') {append_array(&tokens, new_token(TOKEN_GTEQU, line, '\0')); i++;}
			else append_array(&tokens, new_token(TOKEN_GTE, line, '\0'));
			break;
		// Control flow
		case '(':
			append_array(&tokens, new_token(TOKEN_LPAREN, line, '\0'));
			break;
		case ')':
			append_array(&tokens, new_token(TOKEN_RPAREN, line, '\0'));
			break;
		case '[':
			append_array(&tokens, new_token(TOKEN_LBRACKET, line, '\0'));
			break;
		case ']':
			append_array(&tokens, new_token(TOKEN_RBRACKET, line, '\0'));
			break;
		default:
			if (isalpha(text[i]) || text[i] == '_') { // If the character is a letter or underscore, then add it as a 'name' token
				char* name = malloc(1);
				int count = 0;
				while (isalpha(text[i]) || text[i] == '_' || isdigit(text[i])) { // Allow for digits
					name[count] = text[i];
					i++;
					count++;
					name = realloc(name, count + 1);
				}
				name[count] = '\0';
				append_array(&tokens, new_token(TOKEN_NAME, line, name));
				if (text[i] != ' ') i--;
			} else if (isdigit(text[i])) { // If the character is a letter or underscore, then add it as a 'name' token
				char* name = malloc(1);
				int count = 0;
				int ntype = TOKEN_INTEGER;
				while ((text[i] == '.' && text[i+1] != '.') || isdigit(text[i])) { // Allow for decimals, but not more than one
					name[count] = text[i];
					i++;
					count++;
					name = realloc(name, count + 1);
					if (text[i] == '.' && text[i+1] != '.') ntype = TOKEN_FLOAT;
				}
				name[count] = '\0';
				append_array(&tokens, new_token(ntype, line, name));
				if (text[i] != ' ') i--;
			}
			break;
		}
	}
	append_array(&tokens, new_token(TOKEN_EOF, line, '\0'));
	return tokens.arr;
}
