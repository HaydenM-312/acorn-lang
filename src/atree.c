#include "amain.h"

struct TREE {
	Token node;
	Tree* children;
};

Tree make_ast(Token tokens[]) {
	Tree ast;
	ast.node = tokens[0];
	return ast;
}  
