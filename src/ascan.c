#pragma once
#include "amain.h"

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
	TOKEN_RARROW,
	TOKEN_LARROW,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_EOF,
	TOKEN_SOF,
	TOKEN_USING
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
	"TOKEN_RARROW",
	"TOKEN_LARROW",
	"TOKEN_COMMA",
	"TOKEN_DOT",
	"TOKEN_EOF",
	"TOKEN_SOF",
	"TOKEN_USING"
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

char* load_txt(char path[]) {
	FILE *fptr = fopen(path, "r");
	char* file_text;
	size_t size;
	size_t n = 0;
	int c;

	// If the file doesn't exist then return an error and exit the program
	if (!fptr) {
		fprintf(stderr, "No such file or directory found '%s'.", path);
		exit(-1);
	}

	fseek(fptr, 0L, SEEK_END); // Seek to end of file
	size = ftell(fptr);	// Set the size to the offset
	fseek(fptr, 0L, SEEK_SET); // Seek to beginning of file for loading into array
	file_text = (char*)malloc(size); // Allocate memory based on the size of the file

	if (!file_text) {
		fprintf(stderr, "Not enough memory");
		exit(-1);
	}

	while ((c = fgetc(fptr)) != EOF)
		file_text[n++] = (char)c; // Read file and write it into the array character by character
	file_text[n] = '\0'; // Null-terminate the array

	if(!file_text) {
		fprintf(stderr, "Unable to read file");
		exit(-1);
	}

	fclose(fptr); // Close the file

	return file_text; // This returns a malloc'ed value, which NEEDS TO BE FREED
}

Token* tokenize(char path[], int t) {
	char* text;
	if(t == 0) text = load_txt(path);
	else text = path;
	TokenArray tokens;
	long line = 1;
	long character = 1;
	init_array(&tokens);
	for(size_t i = 0; text[i] != '\0'; i++) {
		switch (text[i]) {
		// Misc
		case '\n': // Increment line variable on newline, for better error handling
			line++;
			break;
		case ' ':
		case '\t': break;
		case ';':
			while(text[i+1] != '\n') i++;
		case '.':
			if (text[i+1] == '.') {append_array(&tokens, new_token(TOKEN_RANGE, line, '\0')); i++;}
			else append_array(&tokens, new_token(TOKEN_DOT, line, '\0'));
			break;
		case ',':
			append_array(&tokens, new_token(TOKEN_COMMA, line, '\0'));
			break;
		case '"': {
			char* name = malloc(1);
			int count = 0;
			i++;
			while (text[i] != '"' && text[i] != '\n' && text[i] != '\0') { // Allow for decimals, but not more than one
				if (text[i] != '\\') name[count] = text[i];
				else {
					switch (text[i+1]) {
						case 'n': name[count] = '\n'; i++; break;
						case 't': name[count] = '\t'; i++; break;
						case '"': name[count] = '"'; i++; break;
						case '\\': name[count] = '\\'; i++; break;
						default: i++; break;
					}
				}
				count++;
				i++;
				name = realloc(name, count + 1);
			} if (text[i] == '\n' || text[i] == '\0') {
				err = 1;
				fprintf(stderr, "Unterminated string at line %lu\n", line);
				name[0] = '\0';
				i--;
			} else name[count] = '\0';
			i++;
			append_array(&tokens, new_token(TOKEN_STRING, line, name));
		}
		case '|':
			if (text[i+1] == '>') {append_array(&tokens, new_token(TOKEN_NULLJOINER, line, '\0')); i++;}
			break;
		// Basic arithmatic
		case '+':
			append_array(&tokens, new_token(TOKEN_PLUS, line, '\0'));
			break;
		case '-':
			if (text[i+1] == '>') {append_array(&tokens, new_token(TOKEN_RARROW, line, '\0')); i++;}
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
			else if (text[i+1] == '-') { append_array(&tokens, new_token(TOKEN_LARROW, line, '\0')); i++;}
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
				if (!strcmp(name, "let")) append_array(&tokens, new_token(TOKEN_LET, line, '\0'));
				else if (!strcmp(name, "set")) append_array(&tokens, new_token(TOKEN_SET, line, '\0'));
				else if (!strcmp(name, "if")) append_array(&tokens, new_token(TOKEN_IF, line, '\0'));
				else if (!strcmp(name, "then")) append_array(&tokens, new_token(TOKEN_THEN, line, '\0'));
				else if (!strcmp(name, "elif")) append_array(&tokens, new_token(TOKEN_ELIF, line, '\0'));
				else if (!strcmp(name, "else")) append_array(&tokens, new_token(TOKEN_ELSE, line, '\0'));
				else if (!strcmp(name, "and")) append_array(&tokens, new_token(TOKEN_AND, line, '\0'));
				else if (!strcmp(name, "or")) append_array(&tokens, new_token(TOKEN_OR, line, '\0'));
				else if (!strcmp(name, "case")) append_array(&tokens, new_token(TOKEN_CASE, line, '\0'));
				else if (!strcmp(name, "match")) append_array(&tokens, new_token(TOKEN_MATCH, line, '\0'));
				else if (!strcmp(name, "using")) append_array(&tokens, new_token(TOKEN_USING, line, '\0'));
				else append_array(&tokens, new_token(TOKEN_NAME, line, name));
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
	free(text); // Free the malloc'ed file
	return tokens.arr;
}
