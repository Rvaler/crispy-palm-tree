#if !defined(AST_H)
#define AST_H

#include "hash.h"
#include <stdio.h>

#define MAX_SONS 4

#define AST_PROGRAM 1
#define AST_SYMBOL 2 // TODOS LITERAIS, FOLHAS

/// Operadores aritmeticos
#define AST_ADD 3
#define AST_SUB 4
#define AST_DIV 5
#define AST_MULT 6

/// Globais
#define AST_VAR 7
#define AST_ARRAY 20

/// Tipos
#define AST_BOOL 8
#define AST_CHAR 9
#define AST_INT 10
#define AST_FLOAT 11

/// Operadores logicos
#define AST_OR 12
#define AST_AND 13
#define AST_NE 14
#define AST_EQ 15
#define AST_GE 16
#define AST_LE 17
#define AST_LT 18
#define AST_GT 19

/// Lists
#define AST_LITERAL_LIST 21
#define AST_ARGUMENTS_LIST 23
#define AST_PRINT_LIST 33
#define AST_EXPRESSION_LIST 34

/// Funções
#define AST_FUNCTION 22

/// Argumentos
#define AST_ARGUMENT 24

/// Comandos
#define AST_COMMAND 25
#define AST_READ 26
#define AST_COMMANDS_BLOCK 27
#define AST_COMMANDS_LIST 28
#define AST_RETURN 29
#define AST_FOR 30
#define AST_FOR_TO 140
#define AST_IF 31
#define AST_PRINT 32

/// Atribuicao
#define AST_ATTR 128
#define AST_ATTR_VECTOR 138

/// Identificadores
#define AST_IDENT 135
#define AST_IDENT_ARR 136
#define AST_IDENT_FUN 137

#define AST_PARENTHESES 139

// TEM QUE DEFINIR OS TIPOS DE NODOS, NAO ENTENDI BEM ESSA PARTE

typedef struct ast_node
{
	int type;
	HASH_NODE *symbol;
	struct ast_node *sons[MAX_SONS];

	int lineNumber;
	int dataType;
} AST_NODE;

AST_NODE *astCreate(int type, int lineNumber, HASH_NODE *symbol, AST_NODE *son0, AST_NODE *son1, AST_NODE *son2, AST_NODE *son3);
void astPrintNode(AST_NODE *node);
void astreePrint(AST_NODE *node, int level, FILE *output);

#endif
