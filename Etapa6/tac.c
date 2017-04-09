#include "tac.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

TAC *greatJoin(int numberOfArgs, ...);
void printTacNode(TAC *tac);

// Code Generation
TAC* makeBinOp(int op, TAC** code);
TAC* makeAttr(HASH_NODE* res, TAC **code);
TAC* makeAttrVector(HASH_NODE* res, TAC **code);
TAC* makeIfThen(TAC** code);
TAC* makeIfThenElse(TAC** code);
TAC* makeFor(TAC **code);
TAC* makeForTo(HASH_NODE* res, TAC **code);
TAC* makeRead(HASH_NODE* identifier);
TAC* makeReturn(TAC **code);
TAC* makePrintList(TAC **code);
TAC* makeFunctionDef(HASH_NODE *identifier, TAC** code);
TAC* makeFunctionCall(AST_NODE *functionCall);
TAC* makeVar(HASH_NODE* res, TAC **code);
TAC* makeArray(HASH_NODE* res, TAC **code);
TAC* makeArrayInit(HASH_NODE* res, TAC **code);
TAC* makeIdentVector(HASH_NODE* res, TAC **code) ;
const char* getStringType(int type);

//-----------------------------------------------------------


TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {

	TAC* newTac;
	if(!(newTac = (TAC*) calloc(1, sizeof(TAC))) ){
		fprintf(stderr, "ERROR TAC CREATE: out of memory! \n");
		exit(1);
	}

	newTac->type = type;
	newTac->res = res;
	newTac->op1 = op1;
	newTac->op2 = op2;
	newTac->prev = NULL;
	newTac->next = NULL;

	return newTac;
}

TAC *tacJoin(TAC *a, TAC *b){
	TAC* temp;
	if(!a) { return b; }
	if(!b) { return a; }

	for (temp = b; temp->prev; temp = temp->prev)
	{
		; //vazio, serve para ir até a raiz
	}

	temp->prev = a;
	a->next = temp;
	return b;
}
// invert the tac list at the end of tac generate
TAC *tacInvert(TAC *tacNode) {
	TAC *temp = tacNode;

	if(!tacNode)
		return NULL;

	while(temp->prev)
		temp = temp->prev;

	return temp;
}

void tacPrintListPrev(TAC* node){
	if(node) {
		printTacNode(node);
		tacPrintListPrev(node->prev);
	}
}
void tacPrintListNext(TAC* node) {
	if(node) {
		printTacNode(node);
		tacPrintListNext(node->next);
	}
}

void printTacNode(TAC *tac) {

	if(!tac) {
		printf("EMPTY TAC\n");
		return;
	}
//	printf("TAC type %d\n", tac[->type);
    if(tac->type != TAC_SYMBOL){
        printf("------TAC----\n");
        printf("TAC type %s\n", getStringType(tac->type));
        printf("res %s\n", tac->res?tac->res->text:"not found");
        printf("op1 %s\n", tac->op1?tac->op1->text:"not found");
        printf("op2 %s\n", tac->op2?tac->op2->text:"not found");
	}
}

TAC *tacGenerateCode(AST_NODE *node){

	if(!node){ return 0; }
	int i;
	TAC *code[4]; //max_sons

	// calls the generate for sons in the treatmente for arguments
	if(node->type == AST_IDENT_FUN)
		return makeFunctionCall(node);

	for (i = 0; i < 4; i++) {
		code[i] = 0; // init
	}
	for (i = 0; i < 4; i++) {
		code[i] = tacGenerateCode(node->sons[i]);
	}

	switch(node->type) {
		case AST_SYMBOL:
		case AST_IDENT:
            return tacCreate(TAC_SYMBOL, node->symbol, NULL, NULL); break;
		case AST_IDENT_ARR:
			return makeIdentVector(node->symbol, code); break;
		case AST_FUNCTION:
			return makeFunctionDef(node->symbol, code); break;

		// Binary Operations
		case AST_ADD:
			return makeBinOp(TAC_ADD, code); break;
		case AST_SUB:
			return makeBinOp(TAC_SUB, code); break;
		case AST_MULT:
			return makeBinOp(TAC_MUL, code); break;
		case AST_DIV:
			return makeBinOp(TAC_DIV, code); break;
		case AST_OR:
			return makeBinOp(TAC_OR, code); break;
		case AST_AND:
			return makeBinOp(TAC_AND, code); break;
		case AST_NE:
			return makeBinOp(TAC_NE, code); break;
		case AST_EQ:
			return makeBinOp(TAC_EQ, code); break;
		case AST_GE:
			return makeBinOp(TAC_GE, code); break;
		case AST_LE:
			return makeBinOp(TAC_LE, code); break;
		case AST_LT:
			return makeBinOp(TAC_LT, code); break;
		case AST_GT:
			return makeBinOp(TAC_GT, code); break;
		//Attribution
		case AST_ATTR:
			return makeAttr(node->symbol, code); break;
		case AST_ATTR_VECTOR:
			return makeAttrVector(node->symbol, code); break;
		//Conditions
		case AST_IF:
			return makeIfThen(code); break;
		case AST_IF_ELSE:
			return makeIfThenElse(code); break;
		case AST_FOR:
			return makeFor(code); break;
		case AST_FOR_TO:
			return makeForTo(node->symbol, code); break;

		//Commands
		case AST_READ:
			return makeRead(node->symbol); break;
		case AST_RETURN:
			return makeReturn(code); break;
		case AST_PRINT_LIST:
			return makePrintList(code); break;

        case AST_VAR:
            return makeVar(node->symbol, code); break;
        case AST_ARRAY:
            return makeArray(node->symbol, code); break;
        case AST_LITERAL_LIST:
            return makeArrayInit(node->sons[0]->symbol, code); break;
            break;
	}

	//default return
	return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
}

TAC *greatJoin(int numberOfArgs, ...){
	// join of any number of tacs
	int i;
	va_list args;
	TAC *joinResult = NULL;
	va_start(args, numberOfArgs);
	for (i = 0; i < numberOfArgs; i++) {
		TAC* tac = va_arg(args, TAC*);
		joinResult = tacJoin(joinResult, tac);
	}
	return joinResult;
}

// testar afu isso aqui hahaha!!!!
TAC* makePrintList(TAC **code){
	TAC *tacExpression = code[0];
	TAC *tacListOfPrint = code[1];

	if(!tacExpression) // no expression to print
		return NULL;

	if(!tacListOfPrint){ // no more prints
		TAC *tacPrint = tacCreate(TAC_PRINT, NULL, tacExpression?tacExpression->res:NULL, NULL);
		return tacJoin(tacExpression, tacPrint);
	}

	// creates TAC Print from expression->res
	TAC *tacPrint = tacCreate(TAC_PRINT, NULL, tacExpression?tacExpression->res:NULL, NULL);
	return greatJoin(3, tacExpression, tacPrint, tacListOfPrint);
}
TAC* makeReturn(TAC **code){
	TAC *tacExpression = code[0];
	// tac that indicates the return of function
	TAC *tacReturn = tacCreate(TAC_RET, NULL, tacExpression?tacExpression->res:NULL, NULL);
	return greatJoin(2, tacExpression, tacReturn);
}
TAC* makeReadVector(HASH_NODE *identifier, TAC** code) {
	TAC *tacExpression = code[0];
	TAC *newTac = tacCreate(TAC_READ_VECTOR, makeTemp(), identifier, tacExpression?tacExpression->res:NULL);
	return greatJoin(2, tacExpression, newTac);
}
TAC* makeRead(HASH_NODE *identifier){
	return tacCreate(TAC_READ, NULL, identifier?identifier:NULL, NULL);
}
TAC* makeForTo(HASH_NODE* res, TAC **code){

	// TAC *tacExpressionInitialValue = code[0];
	TAC *tacExpressionFinalValue = code[1];
	TAC *tacLoopCmd = code[2];

	// attribution in the beggining of for_to. ex.: for(i = 0 to 5);
	TAC *tacAttr = makeAttr(res, code);
	// label before condition
	HASH_NODE *labelBeforeForToCondition = makeLabel();
	TAC *tacLabelBeforeForToCondition = tacCreate(TAC_LABEL, labelBeforeForToCondition, NULL, NULL);
	// condition of for_to
	TAC *tacCondition = tacCreate(TAC_LE, makeTemp(), res, tacExpressionFinalValue?tacExpressionFinalValue->res:NULL);
	// add one to var in the end of for execution
	// TAC *oneSymbol = tacCreate // SE PA USAR UM TAC QUE SIMULE O ADD1
//	HASH_NODE *constantOne = hashInsert(SYMBOL_LIT, "1", DATATYPE_INT, -1);
//	TAC *tacAddOne = tacCreate(TAC_ADD, makeTemp(), res, constantOne);
//
//	TAC *tacAttrIncrementLoop = tacCreate(TAC_ATTR, res, tacAddOne?tacAddOne->res:NULL, NULL);
	TAC *tacIncrement = tacCreate(TAC_INCREMENT, res, NULL, NULL);
	// label after for execution
	HASH_NODE *labelAfterForToLoop = makeLabel();
	TAC *tacLabelAfterForToLoop = tacCreate(TAC_LABEL, labelAfterForToLoop, NULL, NULL);
	// test condition, if false jumps to labelAfterForToLoop
	TAC *tacIfz = tacCreate(TAC_IFZ, labelAfterForToLoop, tacCondition?tacCondition->res:NULL, NULL);
	// jumps to condition verification in the beggining of for_to
	TAC *tacJumpToBeginOfForTo = tacCreate(TAC_JUMP, labelBeforeForToCondition, NULL, NULL);

//	return greatJoin(8, tacAttr, tacLabelBeforeForToCondition, tacCondition, tacIfz, tacLoopCmd,
//		tacAddOne, tacAttrIncrementLoop, tacJumpToBeginOfForTo, tacLabelAfterForToLoop);
    return greatJoin(8, tacAttr, tacLabelBeforeForToCondition, tacCondition, tacIfz, tacLoopCmd,
		tacIncrement, tacJumpToBeginOfForTo, tacLabelAfterForToLoop);
	// return NULL
}
TAC *makeArgs(AST_NODE *args, AST_NODE *expressions) {
	// return list of tac of arguments
	TAC *tacListOfArgs = NULL;
	while(args && expressions) {
		// goes through every param and arg
		HASH_NODE *argSymbol = args->sons[0]->symbol;
		AST_NODE *expr = expressions->sons[0];
		// generate code for expressions list in the func call
		TAC *tacExpression = tacGenerateCode(expr);
		// create TAC_ARG
		TAC *tacArg = tacCreate(TAC_ARG, argSymbol, tacExpression?tacExpression->res:NULL, NULL);
		// append the new TAC_ARG with the list of already existed TAC_ARGS
		tacListOfArgs = greatJoin(3, tacListOfArgs, tacExpression, tacArg);

		args = args->sons[1];
		expressions = expressions->sons[1];
	}
	return tacListOfArgs;
}
TAC* makeFunctionCall(AST_NODE *functionCall) {
	// find function definition to get the list of params
	AST_NODE *functionDefinition = findFuncDeclaration(functionCall);
	AST_NODE *expressions = functionCall->sons[0]; // list of expressions in the func call
	AST_NODE *args = functionDefinition->sons[1]; // list of params of the func called

	TAC *tacArgs = makeArgs(args, expressions);
	TAC *tacCall = tacCreate(TAC_CALL, makeTemp(), functionCall->symbol, NULL);
	return greatJoin(2, tacArgs, tacCall);
}
TAC* makeFunctionDef(HASH_NODE *identifier, TAC** code) {
	TAC *funcBody = code[2];
	TAC *tacBeginFunc = tacCreate(TAC_BEGIN_FUNC, identifier, NULL, NULL);
	TAC *tacEndFunc = tacCreate(TAC_END_FUNC, identifier, NULL, NULL);
	return greatJoin(3, tacBeginFunc, funcBody, tacEndFunc);
}
TAC* makeFor(TAC **code){
	TAC *tacExpression = code[0];
	TAC *tacLoopCmd = code[1];
	// label before condition
	HASH_NODE *labelBeforeForCondition = makeLabel();
	TAC *tacLabelBeforeForCondition = tacCreate(TAC_LABEL, labelBeforeForCondition, NULL, NULL);
	// label after the loop commands
	HASH_NODE *labelAfterForLoop = makeLabel();
	TAC *tacLabelAfterLoop = tacCreate(TAC_LABEL, labelAfterForLoop, NULL, NULL);
	// test condition true or false, if false jumps to labelAfterForLoop
	TAC *tacIfz = tacCreate(TAC_IFZ, labelAfterForLoop, tacExpression?tacExpression->res:NULL, NULL);
	// jumps to condition in the beggining of for loop
	TAC *tacJumpToBeginOfFor = tacCreate(TAC_JUMP, labelBeforeForCondition, NULL, NULL);

	return greatJoin(6, tacLabelBeforeForCondition, tacExpression, tacIfz, tacLoopCmd,
						tacJumpToBeginOfFor, tacLabelAfterLoop);
}
TAC* makeIfThenElse(TAC** code){
	TAC *tacExpression = code[0];
	TAC *tacThen = code[1];
	TAC *tacElse = code[2];
	// label after "then" and before "else"
	HASH_NODE *labelAfterThen = makeLabel();
	TAC* tacLabelAfterThen = tacCreate(TAC_LABEL, labelAfterThen, NULL, NULL);
	// label after "else" code
	HASH_NODE *labelAfterElse = makeLabel();
	TAC* tacLabelAfterElse = tacCreate(TAC_LABEL, labelAfterElse, NULL, NULL);
	// if = true, continues where it is
	// if = false, goes to labelAfterThen point
	TAC *tacIfz = tacCreate(TAC_IFZ, labelAfterThen, tacExpression?tacExpression->res:NULL, NULL);
	// if IFZ condition true, have to jump to after else code point
	TAC *tacJumpToAfterElse = tacCreate(TAC_JUMP, labelAfterElse, NULL, NULL);

	return greatJoin(7, tacExpression, tacIfz, tacThen, tacJumpToAfterElse,
						tacLabelAfterThen, tacElse, tacLabelAfterElse);
}
TAC* makeIfThen(TAC** code){
	TAC *tacExpression = code[0];
	TAC *tacThen = code[1];
	// label indicating after "then" point
	HASH_NODE *labelAfterThen = makeLabel();
	TAC *tacLabelAfterThen = tacCreate(TAC_LABEL, labelAfterThen, NULL, NULL);
	// if = true, continues where it is
	// if = false, goes to labelAfterThen point
	TAC *tacIfz = tacCreate(TAC_IFZ, labelAfterThen, tacExpression?tacExpression->res:NULL, NULL);
	return greatJoin(4, tacExpression, tacIfz, tacThen, tacLabelAfterThen);
}
TAC* makeAttrVector(HASH_NODE* res, TAC **code){
	TAC *newTac = tacCreate(TAC_ATTR_VECTOR, res, code[0]?code[0]->res:NULL, code[1]?code[1]->res:NULL);
	return greatJoin(3, code[0], code[1], newTac);
}
TAC* makeIdentVector(HASH_NODE* res, TAC **code) {
    TAC *newTac = tacCreate(TAC_IDENT_ARR, makeTemp(), res, code[0]?code[0]->res:NULL);
	return greatJoin(3, code[0], code[1], newTac);
}
TAC* makeAttr(HASH_NODE* res, TAC **code){
	TAC *newTac = tacCreate(TAC_ATTR, res, code[0]?code[0]->res:NULL, NULL);
	return greatJoin(2, code[0], newTac);
}
TAC* makeBinOp(int op, TAC** code){
	TAC *newTac = tacCreate(op, makeTemp(), code[0]?code[0]->res:NULL, code[1]?code[1]->res:NULL);
	return greatJoin(3, code[0], code[1], newTac);
}
TAC* makeVar(HASH_NODE* res, TAC **code){
	TAC *newTac = tacCreate(TAC_VAR, res, code[1]?code[1]->res:NULL, NULL);
	return greatJoin(2, code[1], newTac);
}
TAC* makeArrayInit(HASH_NODE* res, TAC **code){
    TAC *newTac = tacCreate(TAC_ARRAY_INIT, res, NULL, NULL);
    return greatJoin(2, newTac, code[1]);
}
TAC* makeArray(HASH_NODE* res, TAC **code){
    TAC *arrayTac = tacCreate(TAC_ARRAY, res, code[1]?code[1]->res:NULL, code[2]?makeLabelArrayInit(res):NULL);
    if(code[2])
        return greatJoin(3, code[1], arrayTac, code[2]);
    else
        return greatJoin(2, code[1], arrayTac);
}
const char* getStringType(int type){
    switch(type){
        case  TAC_SYMBOL:
            return "TAC_SYMBOL";
        case  TAC_LABEL:
            return "TAC_LABEL";

        case  TAC_ADD:
            return "TAC_ADD";
        case  TAC_SUB:
            return "TAC_SUB";
        case  TAC_MUL:
            return "TAC_MUL";
        case  TAC_DIV:
            return "TAC_DIV";

        case  TAC_OR:
            return "TAC_OR";
        case  TAC_AND:
            return "TAC_AND";
        case  TAC_NE:
            return "TAC_NE";
        case  TAC_EQ:
            return "TAC_EQ";
        case  TAC_GE:
            return "TAC_GE";
        case  TAC_LE:
            return "TAC_LE";
        case  TAC_LT:
            return "TAC_LT";
        case  TAC_GT:
            return "TAC_GT";

        case  TAC_ATTR:
            return "TAC_ATTR";
        case  TAC_ATTR_VECTOR:
            return "TAC_ATTR_VECTOR";

        case  TAC_IFZ:
            return "TAC_IFZ";
        case  TAC_JUMP:
            return "TAC_JUMP";
        case  TAC_READ:
            return "TAC_READ";
        case  TAC_RET:
            return "TAC_RET";
        case  TAC_PRINT:
            return "TAC_PRINT";
        case  TAC_READ_VECTOR:
            return "TAC_READ_VECTOR";

        case  TAC_BEGIN_FUNC:
            return "TAC_BEGIN_FUNC";
        case  TAC_END_FUNC:
            return "TAC_END_FUNC";
        case  TAC_ARG:
            return "TAC_ARG";
        case  TAC_CALL:
            return "TAC_CALL";

        case  TAC_VAR:
            return "TAC_VAR";
        case  TAC_ARRAY:
            return "TAC_ARRAY";
        case  TAC_ARRAY_INIT:
            return "TAC_ARRAY_INIT";

        case  TAC_INCREMENT:
            return "TAC_INCREMENT";

        case  TAC_IDENT_ARR:
            return "TAC_IDENT_ARR";

        default:
            return "Type not found";
    }
}
