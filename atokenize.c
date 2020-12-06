#pragma once
#include "main.h"
#include <ctype.h>

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
// This is an incomplete function to do something with a token, in the future this will 
// add to the token list.
void token(char* t, char*** list) {
	printf("%s\n",t); 
}


char** tokenize(char text[]) {
	char** token_list;

	for(size_t i = 0; i < strlen(text); i++) {
		switch (text[i]) {
			case ';':
				while(consume(&i, text) != '\n') {}
				break;
			case '`':
				consume(&i, text);
				while(consume(&i, text) != '`'){}
				break;
			case '+':
				token("+", &token_list);
				break; 
			case '-':
				token("-", &token_list);
				break; 
			case '*':
				token("*", &token_list);
				break; 
			case '=':
				token("=", &token_list);
				break; 
			case '%':
				token("%", &token_list);
				break; 
			case ':':
				token(":", &token_list);
				break; 
			case '>':
				if (peek(i, text) == '=') {
					consume(&i, text);
					token(">=", &token_list);
				}
				else {
					token(">", &token_list);
				}
				break;
			case '<':
				if (peek(i, text) == '=') {
					consume(&i, text);
					token("<=", &token_list);
				}
				else {
					token("<", &token_list);
				}
				break;
			case '/':
				if (peek(i, text) == '=') {
					consume(&i, text);
					token("/=", &token_list);
				}
				else {
					token("/", &token_list);
				}
				break;
			default:
				
				break; 
		}
	}
	return token_list;
}