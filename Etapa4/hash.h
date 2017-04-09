#if !defined(HASH_H)
#define HASH_H

#define SYMBOL_LIT 1
#define SYMBOL_SCALAR 2
#define SYMBOL_VECTOR 3
#define SYMBOL_FUNCTION 4
#define SYMBOL_NOT_DEFINED 5

#define DATATYPE_INT 7
#define DATATYPE_CHAR 8
#define DATATYPE_FLOAT 9
#define DATATYPE_BOOL 10
#define DATATYPE_STRING 11 // fiquei confuso nesse, pq ele diz que existem quatro tipos de dados e string n é um deles...
#define DATATYPE_NOT_DEFINED 12

#define HASH_SIZE 997 // tem que ser primo

typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node * next;

	int lineNumber;
	int dataType;
} HASH_NODE;

HASH_NODE* Table[HASH_SIZE];

void hashInit(void);
int hashAddress(char *text);
HASH_NODE* hashInsert(int type, char* text, int dataType, int lineNumber); 
HASH_NODE* hashFind(char *text);
void hashPrint(void);

#endif
