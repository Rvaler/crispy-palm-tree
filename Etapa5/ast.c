#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

AST_NODE *astCreate(int type, int lineNumber, HASH_NODE *symbol, AST_NODE* son0, AST_NODE* son1, AST_NODE* son2, AST_NODE* son3){
	AST_NODE *newNode;
	if(!(newNode = (AST_NODE*) calloc(1, sizeof(AST_NODE))) ){
		fprintf(stderr, "ERROR AST CREATE: out of memory! \n");
		exit(1);
	}

	newNode->type = type;
	newNode->symbol = symbol;
	newNode->dataType = DATATYPE_NOT_DEFINED;
	newNode->lineNumber = lineNumber;
	newNode->sons[0] = son0;
	newNode->sons[1] = son1;
	newNode->sons[2] = son2;
	newNode->sons[3] = son3;

	return newNode;
}

void astPrintNode(AST_NODE *node){
	if(!node){ return; }
	fprintf(stderr, "Node type %i\n", node->type);
	if(node->symbol){
		fprintf(stderr, "Node text %s\n", node->symbol->text);
	} else {
		fprintf(stderr, "Node has no text \n");
	}
}
// só copiei da aula hoje
void astreePrint(AST_NODE *node, int level, FILE* output){
	int i;
	if(node){
//		for(i = 0; i < level; i++){
//			fprintf(output, "  "); //tabulação
//		}
		switch (node->type){
			case AST_PROGRAM:
				astreePrint(node->sons[0], level, output);
				if(node->sons[1]){
					astreePrint(node->sons[1], level, output);
				}
				break;
			case AST_SYMBOL:
				fprintf(output, "%s", node->symbol->text);
			break;
			/// Operação numéricas
			case AST_ADD:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " + ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_SUB:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " - ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_MULT:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " * ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_DIV:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " / ");
				astreePrint(node->sons[1], level, output);
				break;
    	/// Operações lógicas
			case AST_OR:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " || ");
				astreePrint(node->sons[1], level, output);
				break;
  		case AST_AND:
    		astreePrint(node->sons[0], level, output);
				fprintf(output, " && ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_NE:
    		astreePrint(node->sons[0], level, output);
				fprintf(output, " != ");
				astreePrint(node->sons[1], level, output);
				break;
  		case AST_EQ:
    		astreePrint(node->sons[0], level, output);
				fprintf(output, " == ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_GE:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " >= ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_LE:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " <= ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_LT:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " < ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_GT:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " > ");
				astreePrint(node->sons[1], level, output);
				break;
  		/// Tipos
			case AST_BOOL:
				fprintf(output, "bool");
				break;
			case AST_CHAR:
				fprintf(output, "char");
				break;
			case AST_INT:
				fprintf(output, "int");
				break;
			case AST_FLOAT:
				fprintf(output, "float");
				break;
  		/// Globais
			case AST_VAR:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " %s:", node->symbol->text);
				astreePrint(node->sons[1], level, output);
				fprintf(output, ";\n");
			    for(i = 0; i < level; i++){
			     	fprintf(output, "  "); //tabulação
			    }
				break;
			case AST_ARRAY:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " %s", node->symbol->text);
				fprintf(output, "[");
				astreePrint(node->sons[1], level, output);
				fprintf(output, "]");
				if(node->sons[2]){
					fprintf(output, ": ");
					astreePrint(node->sons[2], level, output);
				}
				fprintf(output, ";\n");
				for(i = 0; i < level; i++){
     				fprintf(output, "  "); //tabulação
    			}
				break;
			/// Identificadores
			case AST_IDENT:
				fprintf(output, "%s", node->symbol->text);
				break;
			case AST_IDENT_ARR: // TODO
				fprintf(output, "%s", node->symbol->text);
				fprintf(output, "[");
				astreePrint(node->sons[0], level, output);
				fprintf(output, "]");
				break;
			case AST_IDENT_FUN: // TODO
				fprintf(output, "%s", node->symbol->text);
				fprintf(output, "(");
				if(node->sons[0] != 0) {
					astreePrint(node->sons[0], level, output);
				}
				fprintf(output, ")");
				break;
			/// Atribuicao
			case AST_ATTR:
				fprintf(output, "%s = ", node->symbol->text);
				astreePrint(node->sons[0], level, output);
				break;
			case AST_ATTR_VECTOR:
				fprintf(output, "%s[", node->symbol->text);
				astreePrint(node->sons[0], level, output);
				fprintf(output, "] = ");
				astreePrint(node->sons[1], level, output);
				break;
			/// Funções
			case AST_FUNCTION:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " %s", node->symbol->text);
				fprintf(output, "(");
				if(node->sons[1]){
					astreePrint(node->sons[1], level, output);
				}
				fprintf(output, ")");
				astreePrint(node->sons[2], level, output);
				fprintf(output, ";\n");
				for(i = 0; i < level; i++){
     				fprintf(output, "  "); //tabulação
    			}
				break;
			case AST_ARGUMENTS_LIST:
				astreePrint(node->sons[0], level, output);
				if(node->sons[1]){
					fprintf(output, ", ");
					astreePrint(node->sons[1], level, output);
				}
				break;
			case AST_ARGUMENT:
				astreePrint(node->sons[0], level, output);
				fprintf(output, " %s", node->symbol->text);
				break;
			/// Lists
			case AST_LITERAL_LIST:
				astreePrint(node->sons[0], level, output);
				if(node->sons[1]){
					fprintf(output, " ");
					astreePrint(node->sons[1], level, output);
				}
				break;
			case AST_PRINT_LIST:
				astreePrint(node->sons[0], level, output);
				if(node->sons[1]){
					fprintf(output, " ");
					astreePrint(node->sons[1], level, output);
				}
				break;
			case AST_EXPRESSION_LIST:
				astreePrint(node->sons[0], level, output);
				if(node->sons[1]){
					fprintf(output, ", ");
					astreePrint(node->sons[1], level, output);
				}
				break;
			/// Comandos
			case AST_COMMAND:
				//lots of treatment here, i guess
				if(node != 0) {
					astreePrint(node->sons[0], level, output);
				}
				break;
			case AST_COMMANDS_BLOCK:
     			fprintf(output, "{");
				fprintf(output, "\n");
				for(i = 0; i < level+1; i++){
     				fprintf(output, "  "); //tabulação
    			}
				astreePrint(node->sons[0], level+1, output);
				fprintf(output, "\n");
				for(i = 0; i < level; i++){
     				fprintf(output, "  "); //tabulação
    			}
				fprintf(output, "}");
				break;
			case AST_COMMANDS_LIST:
				astreePrint(node->sons[0], level, output);
				fprintf(output, ";");
				
				if(node->sons[1] != 0){
					fprintf(output, "\n"); 
					for(i = 0; i < level; i++){
	     				fprintf(output, "  "); //tabulação
	    			}
					astreePrint(node->sons[1], level, output);
				}
				break;
			case AST_PRINT:
				fprintf(output, "print ");
				astreePrint(node->sons[0], level, output);
				break;
			case AST_READ:
				fprintf(output, "read %s", node->symbol->text);
				break;
			case AST_RETURN:
				fprintf(output, "return ");
				astreePrint(node->sons[0], level, output);
				break;
			case AST_FOR:
				fprintf(output, "for(");
				astreePrint(node->sons[0], level, output);
				fprintf(output, ")");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_FOR_TO:
				fprintf(output, "for(");
				fprintf(output, "%s = ", node->symbol->text);
				astreePrint(node->sons[0], level, output);
				fprintf(output, " to ");
				astreePrint(node->sons[1], level, output);
				fprintf(output, ") ");
				astreePrint(node->sons[2], level, output);
				break;
			case AST_IF:
				fprintf(output, "if(");
				astreePrint(node->sons[0], level, output);
				fprintf(output, ") then ");
				astreePrint(node->sons[1], level, output);
				break;
			case AST_IF_ELSE:
				fprintf(output, "if(");
				astreePrint(node->sons[0], level, output);
				fprintf(output, ") then ");
				astreePrint(node->sons[1], level, output);
				fprintf(output, " else ");
				astreePrint(node->sons[2], level, output);
				break;
			/// Pontuacoes
			case AST_PARENTHESES:
				fprintf(output, "( ");
				astreePrint(node->sons[0], level, output);
				fprintf(output, " )");
				break;
			default: fprintf(output, "UNKNOWN");
				break;
		}
	}
}



