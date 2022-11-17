/**
 * @file code_generation.h
 * 
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 * 
 * @brief  Decleration of code generation functions and built-in functions
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

static char *generatedString = NULL;

static char *inFunctionString = NULL;

static char *allFunctionsString = NULL; 

static int IAmInFunction = 0;

static int cparCounter = 0;

static int storageLen = 0;

static char **storage = NULL;

static enum type operator = NOT_DEFINED;

void addToString(int str, char *newStr);

void codeGeneration(Token *token);

void store(char *val);

void removeLastFromStorage();

void addToOperator(enum type t);

void removeOperator();

void resetGlobalValues();

void checkStorage();

void threeAddress();
