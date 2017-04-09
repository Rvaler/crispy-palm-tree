#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

LIST_FUNC_DECL* listFuncDecl = 0;

int numberOfErrors;
void setSymbolType(AST_NODE *node);
void setDataType(AST_NODE *node);
int checkAritmeticOperation(int operator1Type, int operator2Type);
int verifyAttributionTypes(int type1, int type2);
void semanticError(int lineNumber, char message[]);

//AST_NODE *declaredFunctions[100];
int functionsCount = 0;

void listFuncDeclInsert(LIST_FUNC_DECL** listFuncDecl, AST_NODE* node){
	LIST_FUNC_DECL *newNode;
	if(!(newNode = (LIST_FUNC_DECL*) calloc(1, sizeof(LIST_FUNC_DECL))) ){
		fprintf(stderr, "ERROR FUNC_DECL_LIST CREATE: out of memory! \n");
		exit(1);
 }

 newNode->ast_node = node;
 newNode->next = 0;

 if(*listFuncDecl == 0)
 {
  *listFuncDecl = newNode;
 }
 else
 {
  LIST_FUNC_DECL* temp = *listFuncDecl;
  while(temp->next != 0)
  {
   temp = temp->next;
  }
  temp->next = newNode;
 }
}


void setSymbolType(AST_NODE *node){
	switch(node->type){
		case AST_ARGUMENT:
			node->symbol->type = SYMBOL_SCALAR; break;
		case AST_VAR:
			node->symbol->type = SYMBOL_SCALAR; break;
		case AST_ARRAY:
			node->symbol->type = SYMBOL_VECTOR; break;
		case AST_FUNCTION:
			node->symbol->type = SYMBOL_FUNCTION; break;
		default:
			node->symbol->type = SYMBOL_NOT_DEFINED; break;
	}
}

void setDataType(AST_NODE *node){
	switch(node->sons[0]->type) {

		case AST_INT:
			node->symbol->dataType = DATATYPE_INT; break;
		case AST_FLOAT:
			node->symbol->dataType = DATATYPE_FLOAT; break;
		case AST_CHAR:
			node->symbol->dataType = DATATYPE_CHAR; break;
		case AST_BOOL:
			node->symbol->dataType = DATATYPE_BOOL; break;
		default:
			node->symbol->dataType = DATATYPE_NOT_DEFINED; break;
	}
}

void checkUndeclaredSymbols() {
	int i;
	HASH_NODE *node;

	for(i = 0; i < HASH_SIZE; i++){
		if( (node = Table[i]) ) {
			while(node) {
				if(node->type == SYMBOL_NOT_DEFINED) {
					semanticError(node->lineNumber, "Undeclared identifier."); // por enquanto só pega a primeira ocorrência do símbolo que não foi declarado
				}
				node = node->next;
			}
		}
	}
}

AST_NODE* findFuncDeclaration(AST_NODE *funcall_node){
 LIST_FUNC_DECL *temp;
 temp = listFuncDecl;
 while(temp != 0)
 {
  if(temp->ast_node->symbol->text == funcall_node->symbol->text)
   return temp->ast_node;
  temp = temp->next;
 }
	return 0;
}

void checkParams(AST_NODE *funcall_node){
	int numberOfParams = 0;

	AST_NODE *funcDeclaration = findFuncDeclaration(funcall_node);
 	if(!funcDeclaration) { // Chamada de função não definida
 		semanticError(funcall_node->lineNumber, "Undeclared function.");
  		return;
 	}

	AST_NODE *nodeFuncCall, *nodeFuncDef;

	nodeFuncCall = funcall_node->sons[0];
	nodeFuncDef = funcDeclaration->sons[1];
	if(nodeFuncCall && nodeFuncDef){
		while(nodeFuncCall && nodeFuncDef){
			numberOfParams++;
			//printf("Node def has type %i, node call has type %i\n", nodeFuncDef->sons[0]->dataType, nodeFuncCall->sons[0]->dataType);
			if(nodeFuncCall->sons[0]->dataType != nodeFuncDef->sons[0]->dataType){
				semanticError(funcall_node->lineNumber, "Function call has type different from its definition.");
			}

			nodeFuncDef = nodeFuncDef->sons[1];
			nodeFuncCall = nodeFuncCall->sons[1];
		}
	}
	if(nodeFuncDef){
		semanticError(funcall_node->lineNumber, "Too few arguments to function.");
	}
	if(nodeFuncCall) {
		semanticError(funcall_node->lineNumber, "Too many arguments to function.");
	}
}

void checkDeclarations(AST_NODE *node) {

	if(!node) { return; }

	if(node->type == AST_VAR || node->type == AST_ARRAY || node->type == AST_FUNCTION || node->type == AST_ARGUMENT) {
		if(node->symbol->type != SYMBOL_NOT_DEFINED){
   if(node->symbol->type == SYMBOL_FUNCTION)
    semanticError(node->lineNumber, "Function was already declared.");
			else
    semanticError(node->lineNumber, "Identifier was already declared.");
		}
		setSymbolType(node);
		setDataType(node);
		node->dataType = node->symbol->dataType;

		if(node->type == AST_FUNCTION) {
   			listFuncDeclInsert(&listFuncDecl, node);
  		}
	}
 	int i;
	for (i = 0; i < 4; i++) {
		checkDeclarations(node->sons[i]);
	}
}

void checkCorrectUse(AST_NODE *node){
	if(!node) { return; }

	switch(node->type) {
		case AST_VAR: // var declaration
			if(node->symbol->type != SYMBOL_SCALAR)
				semanticError(node->lineNumber, "Incorrect use.");
			break;
		case AST_ATTR: // simple attribution
			if(node->symbol->type != SYMBOL_SCALAR)
				semanticError(node->lineNumber, "Incorrect use.");
			break;
		case AST_ATTR_VECTOR: //vector attribution
			if(node->symbol->type != SYMBOL_VECTOR)
				semanticError(node->lineNumber, "Incorrect use.");
			break;
		case AST_IDENT_ARR:
			if(node->symbol->type != SYMBOL_VECTOR)
				semanticError(node->lineNumber, "Incorrect use.");
			break;
		case AST_IDENT_FUN: //function call
			if(node->symbol->type != SYMBOL_FUNCTION)
				semanticError(node->lineNumber, "Incorrect use.");
			break;
	}

	int s;
	for (s = 0; s < 4; s++){
		checkCorrectUse(node->sons[s]);
	}
}

void checkDataTypes(AST_NODE *node){
	if(!node) { return; }

	int s;
	for (s = 0; s < 4; s++){
		checkDataTypes(node->sons[s]);
	}

	switch(node->type) {
	case AST_IDENT:
		node->dataType = node->symbol->dataType;
		break;
	case AST_SYMBOL:
		node->dataType = node->symbol->dataType;
		break;
	case AST_GE:
	case AST_LE:
	case AST_LT:
	case AST_GT:
		if(node->sons[0]->dataType == DATATYPE_BOOL || node->sons[1]->dataType == DATATYPE_BOOL){
			semanticError(node->lineNumber, "Logic Operation - Unexpected bool value.");
		}
		node->dataType = DATATYPE_BOOL;
		break;

	case AST_OR:
	case AST_AND:
		if(node->sons[0]->dataType != DATATYPE_BOOL || node->sons[1]->dataType != DATATYPE_BOOL){
			semanticError(node->lineNumber, "Logic Operation - Expected a bool value.");
		}
		node->dataType = DATATYPE_BOOL;
		break;
	case AST_EQ:
	case AST_NE:
		if((node->sons[0]->dataType == DATATYPE_BOOL && node->sons[1]->dataType != DATATYPE_BOOL) ||
		   (node->sons[1]->dataType == DATATYPE_BOOL && node->sons[0]->dataType != DATATYPE_BOOL)){
			semanticError(node->lineNumber, "Logic Operation - Types conflict.");
		}
		node->dataType = DATATYPE_BOOL;
		break;
	case AST_ADD:
	case AST_SUB:
	case AST_MULT:
		if(node->sons[0]->dataType == DATATYPE_BOOL || node->sons[1]->dataType == DATATYPE_BOOL){
			semanticError(node->lineNumber, "Aritmetic Operation - Unexpected bool value.");
		}
		node->dataType = checkAritmeticOperation(node->sons[0]->dataType, node->sons[1]->dataType);
		break;
	case AST_DIV:
		if(node->sons[0]->dataType == DATATYPE_BOOL || node->sons[1]->dataType == DATATYPE_BOOL){
			semanticError(node->lineNumber, "Aritmetic Operation - Unexpected bool value.");
		}
		node->dataType = DATATYPE_FLOAT;
		break;
	case AST_PARENTHESES:
		node->dataType = node->sons[0]->dataType;
		break;
	case AST_ATTR:
		if(!(verifyAttributionTypes(node->symbol->dataType, node->sons[0]->dataType))){
			semanticError(node->lineNumber, "Attribution type conflict.");
 		}
		break;
	case AST_ATTR_VECTOR:
		if(node->sons[0]->dataType != DATATYPE_INT) {
			semanticError(node->lineNumber, "Vector index access - Expected an integer value.");
		}
		if(!(verifyAttributionTypes(node->symbol->dataType, node->sons[1]->dataType))){
			semanticError(node->lineNumber, "Attribution type conflict.");
		}
		break;
	case AST_IF:
		if(node->sons[0]->dataType != DATATYPE_BOOL){
			semanticError(node->sons[0]->lineNumber, "Command IF - Expected a bool value.");
		}
		break;
	case AST_FOR:
		if(node->sons[0]->dataType != DATATYPE_BOOL){
			semanticError(node->sons[0]->lineNumber, "Command FOR - Expected a bool value.");
		}
		break;
	case AST_FOR_TO:
		if(node->sons[0]->dataType != DATATYPE_INT || node->sons[1]->dataType != DATATYPE_INT){
			semanticError(node->sons[0]->lineNumber, "Command FOR_TO - Expected an integer value.");
		}
		break;
	case AST_IDENT_FUN:
		checkParams(node);
		node->dataType = node->symbol->dataType;
		break;
	}
}


int verifyAttributionTypes(int type1, int type2){
	if(type1 == DATATYPE_NOT_DEFINED || type2 == DATATYPE_NOT_DEFINED) {
		return 0;
	}
	if(type1 == DATATYPE_BOOL && type2 != DATATYPE_BOOL) {
		return 0;
	} else if(type1 != DATATYPE_BOOL && type2 == DATATYPE_BOOL) {
		return 0;
	} else {
		return 1;
	}
}

int checkAritmeticOperation(int operator1Type, int operator2Type){
	if(operator1Type == DATATYPE_BOOL || operator2Type == DATATYPE_BOOL){
		return DATATYPE_NOT_DEFINED;
	}
	switch(operator1Type){
		case DATATYPE_CHAR:
			if(operator2Type == DATATYPE_CHAR){ return DATATYPE_CHAR; }
			if(operator2Type == DATATYPE_INT){ return DATATYPE_INT; }
			if(operator2Type == DATATYPE_FLOAT){ return DATATYPE_FLOAT; }
			break;
		case DATATYPE_FLOAT:
			return DATATYPE_FLOAT;
		case DATATYPE_INT:
			if(operator2Type == DATATYPE_CHAR){ return DATATYPE_INT; }
			if(operator2Type == DATATYPE_INT){ return DATATYPE_INT; }
			if(operator2Type == DATATYPE_FLOAT){ return DATATYPE_FLOAT; }
			break;
	}
	return DATATYPE_NOT_DEFINED;
}

void semanticError(int lineNumber, char message[]){
	numberOfErrors++;
	fprintf(stderr, "(SEMANTIC_ERROR) at %i - %s\n", lineNumber, message);
}


int checkSemantic(AST_NODE *node) {
	fprintf(stderr, "CHECKING SEMANTICS...\n");
	numberOfErrors = 0;
	// check everything

	checkDeclarations(node);
	checkUndeclaredSymbols();
	checkCorrectUse(node);
	checkDataTypes(node);


	return numberOfErrors;
}
