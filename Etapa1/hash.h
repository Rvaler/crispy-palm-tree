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

#define HASH_SIZE 997 // tem que ser primo

typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node * next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE* hashInsert(int type, char* text);
HASH_NODE* hashFind(char *text);
void hashPrint(void);
