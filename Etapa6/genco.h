#if !defined(GENCO_H)
#define GENCO_H

#include <stdio.h>
#include <stdlib.h>
#include "tac.h"
#include "hash.h"
#include "ast.h"

void tac2asm(TAC*node, FILE* output);

#endif