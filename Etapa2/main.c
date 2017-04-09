#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"
#include "hash.h"
#include "y.tab.h"

int yyparse();

int main(int argc, char *argv[]) {
	
	if(argc < 2){
		fprintf(stderr, "Erro na chamada do programa - missing arguments \n" );
		exit(1);
	}
	yyin = fopen(argv[1], "r");
	
	if(!yyin){
		fprintf(stderr, "Cannot open file - %s\n", argv[1]);
		exit(2);
	}
	yyparse();
	fprintf(stderr, "Success!\n");
	hashPrint();
	exit(0);
}
