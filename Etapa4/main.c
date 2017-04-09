#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "lex.yy.h"
#include "y.tab.h"

extern AST_NODE *root;

int yyparse();

int main(int argc, char *argv[]) {

	if(argc < 3){
		fprintf(stderr, "Erro na chamada do programa - missing arguments \n" );
		fprintf(stderr, "Deve ser informado o arquivo de entrada e o arquivo de saida\n" );
		exit(1);
	}
	yyin = fopen(argv[1], "r");

	if(!yyin){
		fprintf(stderr, "Cannot open file - %s\n", argv[1]);
		exit(2);
	}

	yyparse();

  int numberOfSemanticErrors = checkSemantic(root);

 if(numberOfSemanticErrors)
  {
   fprintf(stderr, "\nNumber of semantic erros: %d!\n\n", numberOfSemanticErrors);
   exit(4);
  }

	fprintf(stderr, "\n\nParse Finished!\n\n");

	FILE* output = stdout;
	output = fopen(argv[2], "w");
	if(!output) {
		fprintf(stderr, "Cannot open file - %s\n", argv[2]);
		exit(2);
	}
	astreePrint(root, 0, output);

	fprintf(stderr, "\nWriting Finished!\n\n");
	fclose(output);

	exit(0);
}
