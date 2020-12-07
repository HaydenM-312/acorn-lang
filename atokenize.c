#pragma once
#include "main.h"
#include <ctype.h>

struct Tokens{
	int type;
	long line;
	char* value;
};

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
	TOKEN_DOUBLE,
	TOKEN_INT,
	TOKEN_STRING,
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELIF,
	TOKEN_ELSE,
	TOKEN_LET,
	TOKEN_NIL,
	TOKEN_EOF
};

char peek(size_t index, char array[]) {
	return array[index + 1];
}

char consume(size_t* index, char array[]) {
	return array[(*index)++];
}

int isalphasymbolic(char c) {
	return (isalpha(c) || c == '-' || c == '_');
}

int isdigitoid(char c) {
	return isdigit(c) || c == '.';
}

void print_token(Token t) {
	printf("[type:%d, line:%d", t.type, t.line);
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

void free_token(Token *t) {
	free(t->value);
}
char** tokenize(char text[]) {
	char** token_list;

	for(size_t i = 0; i < strlen(text); i++) {
		switch (text[i]) {
			default:break;
		}
	}
	Token v;
	init_token(&v, TOKEN_STRING, 0, "Hello, friends");
	print_token(v);
	free_token(&v);
	return token_list;
}
