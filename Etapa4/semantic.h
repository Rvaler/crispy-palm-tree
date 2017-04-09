#if !defined(SEMANTIC_H)
#define SEMANTIC_H

#include "hash.h"
#include "ast.h"

typedef struct list_func_declaration
{
 AST_NODE* ast_node;
 struct list_func_declaration* next;
} LIST_FUNC_DECL;

int checkSemantic(AST_NODE *node);
void checkDeclarations(AST_NODE *node);
void checkCorrectUse(AST_NODE *node);
void checkDataTypes(AST_NODE *node);
void checkUndeclaredSymbols();

#endif
