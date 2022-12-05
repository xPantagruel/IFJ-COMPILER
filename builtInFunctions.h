#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "symtable.h"
#include "stdbool.h"
#define DYNAMIC_NUMBER_OF_PARAMS -1

void addBuiltInFunction(char *name, char **paramNames, enum type *paramTypes, bool *paramsCanBeNull, int numberOfParams, enum type returnType, bool canReturnNull);

void addAllBuiltInFunctions();

#endif