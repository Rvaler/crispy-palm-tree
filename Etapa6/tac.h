#if !defined(TAC_H)
#define TAC_H

#include "ast.h"
#include "hash.h"
#include "semantic.h"

// define tac_symbols...

#define TAC_SYMBOL 1
#define TAC_LABEL 2

#define TAC_ADD 3
#define TAC_SUB 4
#define TAC_MUL 5
#define TAC_DIV 6

#define TAC_OR 7
#define TAC_AND 8
#define TAC_NE 9
#define TAC_EQ 10
#define TAC_GE 11
#define TAC_LE 12
#define TAC_LT 13
#define TAC_GT 14

#define TAC_ATTR 15
#define TAC_ATTR_VECTOR 16

#define TAC_IFZ 17
#define TAC_JUMP 18
#define TAC_READ 19
#define TAC_RET 20
#define TAC_PRINT 21
#define TAC_READ_VECTOR 22

#define TAC_BEGIN_FUNC 23
#define TAC_END_FUNC 24
#define TAC_ARG 25
#define TAC_CALL 26

#define TAC_VAR 27
#define TAC_ARRAY 28
#define TAC_ARRAY_INIT 29

#define TAC_INCREMENT 30

#define TAC_IDENT_ARR 31


typedef struct tac_node {
	int type; //move, jump...
	HASH_NODE *res, *op1, *op2;
	struct tac_node *prev, *next;
} TAC;

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC *tac);
TAC *tacJoin(TAC *a, TAC *b);
TAC *tacGenerateCode(AST_NODE *node);
TAC *tacInvert(TAC *tacNode);

void tacPrintListPrev(TAC* node);
void tacPrintListNext(TAC* node);

#endif

