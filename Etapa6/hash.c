#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "hash.h"
#include "y.tab.h"



void hashInit(void){
    int i;
    for(i = 0; i < HASH_SIZE; i++){
        Table[i] = 0;
    }
}

int hashAddress(char *text){
    int i = 0;
    int address = 1;
    for(i = 0; i < strlen(text); i++){
        address = (address * text[i]) % HASH_SIZE + 1;
    }
    return address - 1;
}

HASH_NODE* hashInsert(int type, char* text, int dataType, int lineNumber) {
    int address;
    HASH_NODE *newNode;
    address = hashAddress(text);

    // try to find a node in the hash that is equal to the text entered, if exists, return the newNode with the existent node
    if((newNode = hashFind(text))){
        return newNode;
    }
    newNode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char*) calloc(strlen(text)+1, sizeof(char));
    newNode->lineNumber = lineNumber;
    newNode->dataType = dataType;

    strcpy(newNode->text, text);

    newNode->next = Table[address];
    Table[address] = newNode;
    return newNode;
}
HASH_NODE* hashFind(char *text){
    int address;
    HASH_NODE *node;
    address = hashAddress(text);
    for (node = Table[address]; node ; node = node->next){
        if(!strcmp(text, node->text)){
            return node;
        }
    }
    return 0;
}
void hashPrint(void) {
    int i= 0;
    HASH_NODE * node;
    for(i = 0; i < HASH_SIZE; i++){
        for(node = Table[i]; node; node = node->next){
            fprintf(stderr, "Table %d has %s\n", i, node->text);
        }
    }
}

HASH_NODE *makeTemp(){
    static int tempIndex = 0;
    static char buffer[256]; // verificar se o uso estático n pode dar problema

    sprintf(buffer, "Temp_Identifier_%d", tempIndex);
    tempIndex++;
    return hashInsert(SYMBOL_SCALAR, buffer, DATATYPE_TEMP, 0);

}
HASH_NODE *makeLabel(){
    static int labelIndex = 0;
    static char buffer[256];

    sprintf(buffer, "Label_Identifier_%d", labelIndex);
    labelIndex++;
    return hashInsert(SYMBOL_LABEL, buffer, 0, 0);
}
HASH_NODE *makeLabelArrayInit(HASH_NODE *node){
//    static int labelIndex = 0;
    static char buffer[256];

    sprintf(buffer, "Label_Array_Init_%s", node->text);
//    labelIndex++;
    return hashInsert(SYMBOL_LABEL, buffer, 0, 0);
}
