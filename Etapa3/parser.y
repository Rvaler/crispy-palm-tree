%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "hash.h"
	#include "ast.h"

	int getLineNumber();
	int yyerror();
	int yylex();

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

program	: declaration				{ $$ = astCreate(AST_PROGRAM, NULL, $1, 0, 0, 0); }
		| program declaration		{ $$ = astCreate(AST_PROGRAM, NULL, $1, $2, 0, 0); }
		;

declaration	: function ';'	{ $$ = $1; }
			| global ';'   { $$ = $1; }
			;

global	: variable	{ $$ = $1; }
		| array         { $$ = $1; }
		;

variable: type TK_IDENTIFIER ':' literal { $$ = astCreate(AST_VAR, $2, $1, $4, 0, 0); }
		;

type	: KW_FLOAT  	{ $$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0); }
		| KW_INT      	{ $$ = astCreate(AST_INT, 0, 0, 0, 0, 0); }
		| KW_CHAR     	{ $$ = astCreate(AST_CHAR, 0, 0, 0, 0, 0); }
		| KW_BOOL	    	{ $$ = astCreate(AST_BOOL, 0, 0, 0, 0, 0); }
		;

literal	: LIT_FALSE		{ $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
		| LIT_TRUE	      { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
		| LIT_CHAR	      { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
		| LIT_STRING	    { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
		| literal_integer	{ $$ = $1; }
		;

literal_integer: LIT_INTEGER  { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
		;

array	: type TK_IDENTIFIER '[' literal_integer ']'              	{ $$ = astCreate(AST_ARRAY, $2, $1, $4, 0, 0); }
		| type TK_IDENTIFIER '[' literal_integer ']' ':' literal_list { $$ = astCreate(AST_ARRAY, $2, $1, $4, $7, 0); }
		;

literal_list	: literal     		{ $$ = astCreate(AST_LITERAL_LIST, 0, $1, 0, 0, 0); }
				| literal literal_list	{ $$ = astCreate(AST_LITERAL_LIST, 0, $1, $2, 0, 0); }
				;

function: type TK_IDENTIFIER '(' arguments_list ')' command 	{ $$ = astCreate(AST_FUNCTION, $2, $1, $4, $6, 0); }
		| type TK_IDENTIFIER '(' ')' command                      { $$ = astCreate(AST_FUNCTION, $2, $1, 0, $5, 0); }
		;

arguments_list	: argument          		{ $$ = astCreate(AST_ARGUMENTS_LIST, 0, $1, 0, 0, 0); }
				| argument ',' arguments_list		{ $$ = astCreate(AST_ARGUMENTS_LIST, 0, $1, $3, 0, 0); }
				;

argument: type TK_IDENTIFIER { $$ = astCreate(AST_ARGUMENT, $2, $1, 0, 0, 0); }
		;

command : for_command		 		  { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| if_command				  { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| commands_block              { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| attribution                 { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| read                        { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| print                       { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		| return                      { $$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0); }
		|															{ $$ = 0; }
		;

commands_block	: '{' commands_list '}' { $$ = astCreate(AST_COMMANDS_BLOCK, 0, $2, 0, 0, 0); }
				;

commands_list	: command ';'	    			{ $$ = astCreate(AST_COMMANDS_LIST, 0, $1, 0, 0, 0); }
				| command ';' commands_list 	{ $$ = astCreate(AST_COMMANDS_LIST, 0, $1, $3, 0, 0); }
				;

attribution	: TK_IDENTIFIER '=' expression          		{ $$ = astCreate(AST_ATTR, $1, $3, 0, 0, 0); }
			| TK_IDENTIFIER '[' expression ']' '=' expression { $$ = astCreate(AST_ATTR, $1, $3, $6, 0, 0); }
			;

for_command	: KW_FOR '(' expression ')' command  { $$ = astCreate(AST_FOR, NULL, $3, $5, 0, 0); }
			| KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command  { $$ = astCreate(AST_FOR, $3, $5, $7, $9, 0); }
			;

if_command	: KW_IF '(' expression ')' KW_THEN command  { $$ = astCreate(AST_IF, NULL, $3, $6, 0, 0); }
			| KW_IF '(' expression ')' KW_THEN command KW_ELSE command   { $$ = astCreate(AST_IF, NULL, $3, $6, $8, 0); }
			;

read: KW_READ TK_IDENTIFIER  { $$ = astCreate(AST_READ, $2, 0, 0, 0, 0); }
	;

print 	: KW_PRINT print_list  { $$ = astCreate(AST_PRINT, NULL, $2, 0, 0, 0); }
		;

print_list	: expression print_list { $$ = astCreate(AST_PRINT_LIST, NULL, $1, $2, 0, 0); }
			| expression 			 { $$ = astCreate(AST_PRINT_LIST, NULL, $1, 0, 0, 0); }
			;

return 	: KW_RETURN expression  { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
		;

expression	: TK_IDENTIFIER                   		  { $$ = astCreate(AST_IDENT, $1, 0, 0, 0, 0); }
			| TK_IDENTIFIER '[' expression ']'        { $$ = astCreate(AST_IDENT_ARR, $1, $3, 0, 0, 0); }
			| TK_IDENTIFIER '(' ')'                   { $$ = astCreate(AST_IDENT_FUN, $1, 0, 0, 0, 0); }
			| TK_IDENTIFIER '('  expressions_list ')' { $$ = astCreate(AST_IDENT_FUN, $1, $3, 0, 0, 0); }
			| literal                                 { $$ = $1; }
			| '(' expression ')'                      { $$ = astCreate('(', 0, $2, 0, 0, 0); }
			| expression OPERATOR_LE expression       { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
			| expression OPERATOR_GE expression       { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
			| expression OPERATOR_EQ expression       { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
			| expression OPERATOR_NE expression       { $$ = astCreate(AST_NE, 0, $1, $3, 0, 0); }
			| expression OPERATOR_AND expression      { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
			| expression OPERATOR_OR expression       { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
			| expression '<' expression               { $$ = astCreate(AST_LT, 0, $1, $3, 0, 0); }
			| expression '>' expression               { $$ = astCreate(AST_GT, 0, $1, $3, 0, 0); }
			| expression '+' expression               { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
			| expression '-' expression               { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
			| expression '*' expression               { $$ = astCreate(AST_MULT, 0, $1, $3, 0, 0); }
			| expression '/' expression               { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
			;

expressions_list: expression ',' expressions_list  { $$ = astCreate(AST_EXPRESSION_LIST, NULL, $1, $3, 0, 0); }
				| expression 					   { $$ = astCreate(AST_EXPRESSION_LIST, NULL, $1, 0, 0, 0); }
				;

%%

int yyerror() {
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}
