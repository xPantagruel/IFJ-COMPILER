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

/** String for global frame -> 0 */
static char *generatedString = NULL;

/** String for actual function -> 1 */
static char *inFunctionString = NULL;

/** String which will be printed -> 2 */
static char *allFunctionsString = NULL; 

/** Auxiliary variable to know if we are in function */
static int IAmInFunction = 0;

/** Counter of { and } */
static int cparCounter = 0;

/** Number of items in storage */
static int storageLen = 0;

/** if '=' was found -> 1 otherwise 0 */
static int eqSymbolFound = 0;

/** Storage for expressions (var1 = var2 + 2) */
static char **storage = NULL;

/** Actual operator in expression */
static enum type operator = NOT_DEFINED;

/**
 * @brief Function which append newStr to str.
 * 
 * @param str 0 -> generatedString
              1 -> inFunctionString
              2 -> allFunctionsString
 * @param newStr pointer to string which will be appended
 */
void addToString(int str, char *newStr);

/**
 * @brief "Main" function in code_generation.
 * 
 * @param token token
 */
void codeGeneration(Token *token);

/**
 * @brief Function which we use to store values to storage.
 * 
 * @param val value which will be stored to storage. 
 */
void store(char *val);

/**
 * @brief Function which removes last item from storage.
 */
void removeLastFromStorage();

/**
 * @brief Function which set operator global variable.
 * 
 * @param t operator type
 */
void addToOperator(enum type t);

/**
 * @brief Function which set operator global variable to null.
 */
void removeOperator();

/**
 * @brief Function which reset values of global variables.
 */
void resetGlobalValues();

/**
 * @brief Function which check if is some variable in storage.
          Case: $var1 = 3; -> MOVE GF@var1 int@3
 */
void checkStorage();

/**
 * @brief Function which generates three address code.
 * 
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void threeAddress(int frameStr, char *frame);

/**
 * @brief Function which generates code which pushes vars to stack.
 * 
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void pushStorage(int frameStr, char *frame);

/**
 * @brief Same as pushStorage function but without deleting storage at the end.
 * 
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void pushWithoutDeleting(int frameStr, char *frame);

int convertToSameType(int frameStr, char *frame);

void rand_str(char *dest, size_t length);

/*** End of code_generation.h ***/
