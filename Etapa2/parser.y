%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "hash.h"	

	int getLineNumber();
	int yyerror();
	int yylex();
%}

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

%union
{
	HASH_NODE *symbol;
}

%%

program	: declaration	
		| program declaration
		;

declaration	: function ';'
			| global ';'
			;

global	: variable	
		| array 
		;  

variable: type TK_IDENTIFIER ':' literal
		;

type	: KW_FLOAT 	
		| KW_INT  
		| KW_CHAR  
		| KW_BOOL	
		;

literal	: LIT_FALSE		
		| LIT_TRUE	
		| LIT_CHAR	
		| LIT_STRING	
		| LIT_INTEGER	
		;

array	: type TK_IDENTIFIER '[' LIT_INTEGER ']'
		| type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' literal_list 	
		;

literal_list	: literal
				| literal literal_list
				;

function: type TK_IDENTIFIER '(' arguments_list ')' command
		| type TK_IDENTIFIER '(' ')' command
		;

arguments_list	: argument
				| argument ',' arguments_list
				;

argument: type TK_IDENTIFIER
		;

command : flux_control_command
		| commands_block
		| attribution
		| read
		| print
		| return
		|
		;

commands_block	: '{' ';' '}'
				| '{' commands_list '}'
				;

commands_list	: command ';'
				| command ';' commands_list
				;

attribution	: TK_IDENTIFIER '=' expression
			| TK_IDENTIFIER '[' expression ']' '=' expression
			;

flux_control_command	: for_command
						| if_command
						;

for_command	: KW_FOR '(' expression ')' command				
			| KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command
			;
			
if_command	: KW_IF '(' expression ')' KW_THEN command 
			| KW_IF '(' expression ')' KW_THEN command KW_ELSE command
			;
			
read: KW_READ TK_IDENTIFIER
	;

print 	: KW_PRINT print_list 
		;

print_list	: print_list expression
			| expression
			;

return 	: KW_RETURN expression
		;

expression	: TK_IDENTIFIER
			| TK_IDENTIFIER '[' expression ']'
			| TK_IDENTIFIER '(' ')'
			| TK_IDENTIFIER '(' expressions_list ')'
			| literal
			| '(' expression ')'
			| expression logic_operator expression
			| expression aritmetic_operator expression
			;

expressions_list: expressions_list ',' expression
				| expression
				;

logic_operator	: OPERATOR_LE
				| OPERATOR_GE
				| OPERATOR_EQ
				| OPERATOR_NE
				| OPERATOR_AND
				| OPERATOR_OR
				| '<'
				| '>'
				;

aritmetic_operator	: '+'
					| '-'
					| '*'
					| '/'
					;

%%

int yyerror() {	 
	fprintf(stderr, "Syntax error in line %d.\n", getLineNumber());
	exit(3);
}