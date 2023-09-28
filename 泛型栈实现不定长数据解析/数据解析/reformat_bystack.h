#ifndef _REFORMATSTACK_H
#define _REFORMATSTACK_H

#include "Generics_Stack.h"
#include "string.h"
#include "stdlib.h"

#define startSymbol ':'
#define endSymbol ','
#define NUMBER_OF_RESULT 3

typedef struct ReformatIndex_t
{
    int index;
    size_t length[NUMBER_OF_RESULT];
} ReformatIndex_t;

static int result[NUMBER_OF_RESULT];

void bufferReformat(char *buffer, int *result);

#endif