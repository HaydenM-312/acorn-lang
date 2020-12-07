#pragma once
#include "main.h"
#include <ctype.h>

size_t i;

enum Token_Types {
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
	TOKEN_NUM,
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
	TOKEN_DO,
	TOKEN_EOF,
	TOKEN_SOF
};
char* Token_Types_Print[] = {
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
	"TOKEN_NUM",
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
	"TOKEN_DO",
	"TOKEN_EOF",
	"TOKEN_SOF"
};

struct Tokens {
	int type;
	long line;
	char* value;
};

void print_token(Token t) {
	printf("[type:%s", Token_Types_Print[t.type]);
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

char peek(char array[]) {
	return array[i + 1];
}
int isalphasymbolic(char c) {
	return (isalpha(c) ||  c == '_');
}

int isdigitoid(char c) {
	return isdigit(c) || c == '.';
}


Token* tokenize(char text[]) {
	Token* token_array = malloc(sizeof(Token));
	token_array[0] = new_token(TOKEN_SOF, 0, '\0');
	long token_count = 1;
	long cur_line = 1;
	for(i = 0; i <= strlen(text); i++) {
		switch(text[i]) {
			// Misc operators
			case 0:
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_EOF, cur_line, '\0');
				break;
			case '\n':
				cur_line++;
				break;
			case ';':
				while (text[i] != '\n') i++;
				break;
			case '"': {
				size_t j = i+1;
				i++; while(text[i] != '"') {
					i++;
					if (i > strlen(text)) {
						fprintf(stderr, "Unfinished string at line %d.\n", cur_line);
						err = 1;
						token_count++;
						token_array = realloc(token_array, sizeof(Token) * token_count);
						token_array[token_count-1] = new_token(TOKEN_EOF, cur_line, '\0');

						break;
					}
				}
				if (err) break;
				// Find the size of the number
																// It makes no sense why decrementing i is necesarry here, but who cares if it works
				char* out = malloc(i-j);		//
				size_t sze = i-j;					//
				for(size_t k = 0; k < sze; k++) {
					out[k] = text[j];
					j++;
				}

				out[sze] = '\0';
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_STRING, cur_line, out);
				break;
			}
			case '|':
				if (peek(text) == '>') {
					token_count++;
					token_array = realloc(token_array, sizeof(Token) * token_count);
					token_array[token_count-1] = new_token(TOKEN_NULLJOINER, cur_line, '\0');
					i++;
				}
				break;
			// Basic math operators
			case '%':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_MOD, cur_line, '\0');
				break;
			case '+':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_PLUS, cur_line, '\0');
				break;
			case '-':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				if (peek(text) == '>') {
					token_array[token_count-1] = new_token(TOKEN_ARROW, cur_line, '\0');
					i++;
				} else {
					token_array[token_count-1] = new_token(TOKEN_MINUS, cur_line, '\0');
				}
				break;
			case '*':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_TIMES, cur_line, '\0');
				break;
			case '.':
				if (peek(text) == '.') {
					token_count++;
					token_array = realloc(token_array, sizeof(Token) * token_count);
					token_array[token_count-1] = new_token(TOKEN_RANGE, cur_line, '\0');
					i++;
				}
				break;
			// Brackets
			case '(':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_LPAREN, cur_line, '\0');
				break;
			case ')':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_RPAREN, cur_line, '\0');
				break;
			case '[':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_LBRACKET, cur_line, '\0');
				break;
			case ']':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_RBRACKET, cur_line, '\0');
				break;
			case '/':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				if (peek(text) == '=') {
					token_array[token_count-1] = new_token(TOKEN_NEQ, cur_line, '\0');
					i++;
				}
				else {
					token_array[token_count-1] = new_token(TOKEN_DIV, cur_line, '\0');
				}
				break;
			// Comparison operators
			case '=':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				token_array[token_count-1] = new_token(TOKEN_EQU, cur_line, '\0');
				break;
			case '>':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				if (peek(text) == '=') {
					token_array[token_count-1] = new_token(TOKEN_GTEQU, cur_line, '\0');
					i++;
				}
				else {
					token_array[token_count-1] = new_token(TOKEN_GTE, cur_line, '\0');
				}
				break;
			case '<':
				token_count++;
				token_array = realloc(token_array, sizeof(Token) * token_count);
				if (peek(text) == '=') {
						token_array[token_count-1] = new_token(TOKEN_LTEQU, cur_line, '\0');
					i++;
				}
				else {
					token_array[token_count-1] = new_token(TOKEN_LTE, cur_line, '\0');
				}
				break;
			default:
				if (isalphasymbolic(text[i])) {
					size_t j = i;
					while((isalphasymbolic(text[i]))) i++; // Find the size of the text
					i--;
					char* out = malloc(i-j);
					size_t sze = i-j+1;
					for(size_t k = 0; k < sze; k++) {
						out[k] = text[j];
						j++;
					}
					out[sze] = '\0';
					token_count++;
					token_array = realloc(token_array, sizeof(Token) * token_count);
						if (strcmp(out, "let") == 0) {token_array[token_count-1] = new_token(TOKEN_LET, cur_line, '\0');}
						else if (strcmp(out, "set") == 0) {token_array[token_count-1] = new_token(TOKEN_SET, cur_line, '\0');}
						else if (strcmp(out, "if") == 0) {token_array[token_count-1] = new_token(TOKEN_IF, cur_line, '\0');}
						else if (strcmp(out, "elif") == 0) {token_array[token_count-1] = new_token(TOKEN_ELIF, cur_line, '\0');}
						else if (strcmp(out, "else") == 0) {token_array[token_count-1] = new_token(TOKEN_ELSE, cur_line, '\0');}
						else if (strcmp(out, "then") == 0) {token_array[token_count-1] = new_token(TOKEN_THEN, cur_line, '\0');}
						else if (strcmp(out, "let") == 0) {token_array[token_count-1] = new_token(TOKEN_LET, cur_line, '\0');}
						else if (strcmp(out, "case") == 0) {token_array[token_count-1] = new_token(TOKEN_CASE, cur_line, '\0');}
						else if (strcmp(out, "match") == 0) {token_array[token_count-1] = new_token(TOKEN_MATCH, cur_line, '\0');}
						else if (strcmp(out, "do") == 0) {token_array[token_count-1] = new_token(TOKEN_DO, cur_line, '\0');}
						else if (strcmp(out, "end") == 0 || strcmp(out, "nil") == 0) { token_array[token_count-1] = new_token(TOKEN_NIL, cur_line, '\0');}
						else {token_array[token_count-1] = new_token(TOKEN_NAME, cur_line, out);}
					break;
				} else if (isdigit(text[i])) {
					size_t j = i;
					while(isdigit(text[i]) || (text[i] == '.' && peek(text) != '.')) i++;  // Find the size of the number
					i--;												// It makes no sense why decrementing i is necesarry here, but who cares if it works
					char* out = malloc(i-j);		//
					size_t sze = i-j+1;					//
					for(size_t k = 0; k < sze; k++) {
						out[k] = text[j];
						j++;
					}

					out[sze] = '\0';
					token_count++;
					token_array = realloc(token_array, sizeof(Token) * token_count);
					token_array[token_count-1] = new_token(TOKEN_NUM, cur_line, out);
					break;
				}
		}
	}

	return token_array;
}
