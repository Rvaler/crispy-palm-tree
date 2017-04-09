/****************************************************
** UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL
** INSITUTO DE INFORMÁTICA
** INFO1147 - COMPILADORES (2016/2)
** TURMA A
** 
** Professor:
**     Marcelo de Oliveira Johann
**
**         ETAPA 1 - Análise Léxica	e Inicialização de Tabela de Símbolos
**
** Alunos:
**     Bruno Dias Freitas
**     Rafael Valer
**
****************************************************/

#include <stdio.h>
#include "lex.yy.h"
#include "tokens.h"
#include "hash.h"

//#define DEBUG 1

#ifdef DEBUG
void debug_hashPrint()
{
    fprintf(stderr, "\n------------------ HASH TABLE -------------------------\n");
    hashPrint();
    fprintf(stderr, "\n------------------ END HASH TABLE -------------------------\n");

}
#endif // DEBUG


int main() {

	int tok = 0;
	yyin = fopen("teste.txt", "r");
	initMe();

	while(isRunning()) {



		tok = yylex();
		if(!isRunning()){
			break;
		}

		switch(tok){
			case KW_INT:
				fprintf(stderr, "Found KW_INT - line %i\n", getLineNumber());
				break;
			case KW_FLOAT:
				fprintf(stderr, "Found KW_FLOAT - line %i\n", getLineNumber());
				break;
			case KW_BOOL:
				fprintf(stderr, "Found KW_BOOL - line %i\n", getLineNumber());
				break;
			case KW_CHAR:
				fprintf(stderr, "Found KW_CHAR - line %i\n", getLineNumber());
				break;
			case KW_IF:
				fprintf(stderr, "Found KW_IF - line %i\n", getLineNumber());
				break;
			case KW_THEN:
				fprintf(stderr, "Found KW_THEN - line %i\n", getLineNumber());
				break;
			case KW_ELSE:
				fprintf(stderr, "Found KW_ELSE - line %i\n", getLineNumber());
				break;
            case KW_FOR:
				fprintf(stderr, "Found KW_FOR - line %i\n", getLineNumber());
				break;
			case KW_READ:
				fprintf(stderr, "Found KW_READ - line %i\n", getLineNumber());
				break;
			case KW_PRINT:
				fprintf(stderr, "Found KW_PRINT - line %i\n", getLineNumber());
				break;
			case KW_RETURN:
				fprintf(stderr, "Found KW_RETURN - line %i\n", getLineNumber());
				break;
			case OPERATOR_LE:
				fprintf(stderr, "Found OPERATOR_LE - line %i\n", getLineNumber());
				break;
			case OPERATOR_GE:
				fprintf(stderr, "Found OPERATOR_GE - line %i\n", getLineNumber());
				break;
			case OPERATOR_EQ:
				fprintf(stderr, "Found OPERATOR_EQ - line %i\n", getLineNumber());
				break;
			case OPERATOR_NE:
				fprintf(stderr, "Found OPERATOR_NE - line %i\n", getLineNumber());
				break;
			case OPERATOR_AND:
				fprintf(stderr, "Found OPERATOR_AND - line %i\n", getLineNumber());
				break;
			case OPERATOR_OR:
				fprintf(stderr, "Found OPERATOR_OR - line %i\n", getLineNumber());
				break;

			case LIT_FALSE:
				fprintf(stderr, "Found LIT_FALSE - line %i\n", getLineNumber());
				break;

			case LIT_TRUE:
				fprintf(stderr, "Found LIT_TRUE - line %i\n", getLineNumber());
                break;

			case LIT_INTEGER:
				fprintf(stderr, "Found LIT_INTEGER - line %i\n", getLineNumber());
				#ifdef DEBUG
                debug_hashPrint();
                #endif // DEBUG
				break;

			case LIT_CHAR:
				fprintf(stderr, "Found LIT_CHAR - line %i\n", getLineNumber());
				#ifdef DEBUG
                debug_hashPrint();
                #endif // DEBUG
				break;

			case LIT_STRING:
				fprintf(stderr, "Found LIT_STRING - line %i\n", getLineNumber());
				#ifdef DEBUG
                debug_hashPrint();
                #endif // DEBUG
				break;

			case TK_IDENTIFIER:
				fprintf(stderr, "Found TK_IDENTIFIER - line %i\n", getLineNumber());
				#ifdef DEBUG
                debug_hashPrint();
                #endif // DEBUG
				break;

			case TOKEN_ERROR:
				fprintf(stderr, "Found TOKEN_ERROR - line %i\n", getLineNumber());
				break;
			default:
				fprintf(stderr, "Found %c - line %i\n", tok, getLineNumber());
				break;
		}
	}
	fprintf(stderr, "\n------------------ HASH TABLE -------------------------\n");
	hashPrint();

	return 1;
}
