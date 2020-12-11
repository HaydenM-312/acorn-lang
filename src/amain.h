#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ascan.c"
#include <ctype.h>
#ifndef TOKEN_TYPE
#define TOKEN_TYPE
typedef struct Tokens Token;
#endif
#ifndef TREE_TYPE
#define TREE_TYPE
typedef struct TREE Tree;
#endif
#ifndef ERROR_VAR
#define ERROR_VAR
extern int err = 0;
#endif
