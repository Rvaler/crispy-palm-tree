%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "hash.h"
	#include "ast.h"

	int getLineNumber();
	int yyerror();
	int yylex();

 int funcLineNumber = 0;

	AST_NODE *root;
%}

%union
{
	HASH_NODE *symbol;
	AST_NODE *ast;
}

%token KW_INT
%token KW_FLOAT
%token KW_BOOL
%token KW_CHAR
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_FOR
%token KW_READ
%token KW_RETURN
%token KW_PRINT
%token KW_TO

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token <symbol>TK_IDENTIFIER
%token <symbol>LIT_INTEGER
%token <symbol>LIT_FALSE
%token <symbol>LIT_TRUE
%token <symbol>LIT_CHAR
%token <symbol>LIT_STRING

%token TOKEN_ERROR

%left OPERATOR_OR
%left OPERATOR_AND
%left OPERATOR_EQ OPERATOR_NE
%left '<' '>' OPERATOR_LE OPERATOR_GE
%left '+' '-'
%left '*' '/'

%type<ast> program declaration global variable type literal array literal_list function arguments_list
%type<ast> argument command commands_block commands_list attribution for_command if_command
%type<ast> read print print_list return expression expressions_list literal_integer



%%

root	: program		{ root = $1; }
		;

program	: declaration				{ $$ = astCreate(AST_PROGRAM, getLineNumber(), NULL, $1, 0, 0, 0); }
		| program declaration		{ $$ = astCreate(AST_PROGRAM, getLineNumber(), NULL, $1, $2, 0, 0); }
		;

declaration	: function ';'	{ $$ = $1; }
			| global ';'   { $$ = $1; }
			;

global	: variable	{ $$ = $1; }
		| array         { $$ = $1; }
		;

variable: type TK_IDENTIFIER ':' literal { $$ = astCreate(AST_VAR, getLineNumber(), $2, $1, $4, 0, 0); }
		;

type	: KW_FLOAT  	{ funcLineNumber = getLineNumber(); $$ = astCreate(AST_FLOAT, getLineNumber(), 0, 0, 0, 0, 0); }
		| KW_INT      	{ funcLineNumber = getLineNumber(); $$ = astCreate(AST_INT, getLineNumber(), 0, 0, 0, 0, 0); }
		| KW_CHAR     	{ funcLineNumber = getLineNumber(); $$ = astCreate(AST_CHAR, getLineNumber(), 0, 0, 0, 0, 0); }
		| KW_BOOL	    { funcLineNumber = getLineNumber(); $$ = astCreate(AST_BOOL, getLineNumber(), 0, 0, 0, 0, 0); }
		;

literal	: LIT_FALSE		{ $$ = astCreate(AST_SYMBOL, getLineNumber(), $1, 0, 0, 0, 0); }
		| LIT_TRUE	      { $$ = astCreate(AST_SYMBOL, getLineNumber(), $1, 0, 0, 0, 0); }
		| LIT_CHAR	      { $$ = astCreate(AST_SYMBOL, getLineNumber(), $1, 0, 0, 0, 0); }
		| LIT_STRING	    { $$ = astCreate(AST_SYMBOL, getLineNumber(), $1, 0, 0, 0, 0); }
		| literal_integer	{ $$ = $1; }
		;

literal_integer: LIT_INTEGER  { $$ = astCreate(AST_SYMBOL, getLineNumber(), $1, 0, 0, 0, 0); }
		;

array	: type TK_IDENTIFIER '[' literal_integer ']'              	{ $$ = astCreate(AST_ARRAY, getLineNumber(), $2, $1, $4, 0, 0); }
		| type TK_IDENTIFIER '[' literal_integer ']' ':' literal_list { $$ = astCreate(AST_ARRAY, getLineNumber(), $2, $1, $4, $7, 0); }
		;

literal_list	: literal     		{ $$ = astCreate(AST_LITERAL_LIST, getLineNumber(), 0, $1, 0, 0, 0); }
				| literal literal_list	{ $$ = astCreate(AST_LITERAL_LIST, getLineNumber(), 0, $1, $2, 0, 0); }
				;

function: type TK_IDENTIFIER '(' arguments_list ')' command 	{ $$ = astCreate(AST_FUNCTION, funcLineNumber, $2, $1, $4, $6, 0); }
		| type TK_IDENTIFIER '(' ')' command                      { $$ = astCreate(AST_FUNCTION, funcLineNumber, $2, $1, 0, $5, 0); }
		;

arguments_list	: argument          				{ $$ = astCreate(AST_ARGUMENTS_LIST, getLineNumber(), 0, $1, 0, 0, 0); }
				| argument ',' arguments_list		{ $$ = astCreate(AST_ARGUMENTS_LIST, getLineNumber(), 0, $1, $3, 0, 0); }
				;

argument: type TK_IDENTIFIER { $$ = astCreate(AST_ARGUMENT, getLineNumber(), $2, $1, 0, 0, 0); }
		;

command : for_command		 		  { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| if_command				  { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| commands_block              { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| attribution                 { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| read                        { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| print                       { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		| return                      { $$ = astCreate(AST_COMMAND, getLineNumber(), 0, $1, 0, 0, 0); }
		|															{ $$ = 0; }
		;

commands_block	: '{' commands_list '}' { $$ = astCreate(AST_COMMANDS_BLOCK, getLineNumber(), 0, $2, 0, 0, 0); }
				;

commands_list	: command ';'	    			{ $$ = astCreate(AST_COMMANDS_LIST, getLineNumber(), 0, $1, 0, 0, 0); }
				| command ';' commands_list 	{ $$ = astCreate(AST_COMMANDS_LIST, getLineNumber(), 0, $1, $3, 0, 0); }
				;

attribution	: TK_IDENTIFIER '=' expression          			{ $$ = astCreate(AST_ATTR, getLineNumber(), $1, $3, 0, 0, 0); }
			| TK_IDENTIFIER '[' expression ']' '=' expression 	{ $$ = astCreate(AST_ATTR_VECTOR, getLineNumber(), $1, $3, $6, 0, 0); }
			;

for_command	: KW_FOR '(' expression ')' command  { $$ = astCreate(AST_FOR, getLineNumber(), NULL, $3, $5, 0, 0); }
			| KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command  { $$ = astCreate(AST_FOR_TO, getLineNumber(), $3, $5, $7, $9, 0); }
			;

if_command	: KW_IF '(' expression ')' KW_THEN command  { $$ = astCreate(AST_IF, getLineNumber(), NULL, $3, $6, 0, 0); }
			| KW_IF '(' expression ')' KW_THEN command KW_ELSE command   { $$ = astCreate(AST_IF, getLineNumber(), NULL, $3, $6, $8, 0); }
			;

read: KW_READ TK_IDENTIFIER  { $$ = astCreate(AST_READ, getLineNumber(), $2, 0, 0, 0, 0); }
	;

print 	: KW_PRINT print_list  { $$ = astCreate(AST_PRINT, getLineNumber(), NULL, $2, 0, 0, 0); }
		;

print_list	: expression print_list { $$ = astCreate(AST_PRINT_LIST, getLineNumber(), NULL, $1, $2, 0, 0); }
			| expression 			 { $$ = astCreate(AST_PRINT_LIST, getLineNumber(), NULL, $1, 0, 0, 0); }
			;

return 	: KW_RETURN expression  { $$ = astCreate(AST_RETURN, getLineNumber(), 0, $2, 0, 0, 0); }
		;

expression	: TK_IDENTIFIER                   		  { $$ = astCreate(AST_IDENT, getLineNumber(), $1, 0, 0, 0, 0); }
			| TK_IDENTIFIER '[' expression ']'        { $$ = astCreate(AST_IDENT_ARR, getLineNumber(), $1, $3, 0, 0, 0); }
			| TK_IDENTIFIER '(' ')'                   { $$ = astCreate(AST_IDENT_FUN, getLineNumber(), $1, 0, 0, 0, 0); }
			| TK_IDENTIFIER '('  expressions_list ')' { $$ = astCreate(AST_IDENT_FUN, getLineNumber(), $1, $3, 0, 0, 0); }
			| literal                                 { $$ = $1; }
			| '(' expression ')'                      { $$ = astCreate(AST_PARENTHESES, getLineNumber(), 0, $2, 0, 0, 0); }
			| expression OPERATOR_LE expression       { $$ = astCreate(AST_LE, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression OPERATOR_GE expression       { $$ = astCreate(AST_GE, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression OPERATOR_EQ expression       { $$ = astCreate(AST_EQ, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression OPERATOR_NE expression       { $$ = astCreate(AST_NE, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression OPERATOR_AND expression      { $$ = astCreate(AST_AND, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression OPERATOR_OR expression       { $$ = astCreate(AST_OR, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '<' expression               { $$ = astCreate(AST_LT, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '>' expression               { $$ = astCreate(AST_GT, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '+' expression               { $$ = astCreate(AST_ADD, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '-' expression               { $$ = astCreate(AST_SUB, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '*' expression               { $$ = astCreate(AST_MULT, getLineNumber(), 0, $1, $3, 0, 0); }
			| expression '/' expression               { $$ = astCreate(AST_DIV, getLineNumber(), 0, $1, $3, 0, 0); }
			;

expressions_list: expression ',' expressions_list  { $$ = astCreate(AST_EXPRESSION_LIST, getLineNumber(), NULL, $1, $3, 0, 0); }
				| expression 					   { $$ = astCreate(AST_EXPRESSION_LIST, getLineNumber(), NULL, $1, 0, 0, 0); }
				;

%%

int yyerror() {
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}
