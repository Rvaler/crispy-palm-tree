
#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_IDENTIFIER 4
#define SYMBOL_LIT_FALSE 5
#define SYMBOL_LIT_TRUE 6

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