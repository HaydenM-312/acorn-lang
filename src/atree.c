#pragma once
#include "amain.h"

struct TREE {
	Token* node;
	Tree* children;
};

void make_ast(Token* tokens, Tree* ast) {
	ast->node = (Token*)malloc(sizeof(Token*));
	ast->node = tokens;
	ast->node = (Token*)malloc(sizeof(Token*));
	ast->node = tokens; 
}

void free_ast(Tree* ast) {
	free(ast->node);
}
