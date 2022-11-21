#ifndef CODEGEN_H
#define CODEGEN_H
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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/** Prvek dvousměrně vázaného seznamu. */
typedef struct DLLElement
{
    /** Užitečná data. */
    char *data;
    /** Ukazatel na předcházející prvek seznamu. */
    struct DLLElement *previousElement;
    /** Ukazatel na následující prvek seznamu. */
    struct DLLElement *nextElement;
} * DLLElementPtr;

/** Dvousměrně vázaný seznam. */
typedef struct
{
    /** Ukazatel na první prvek seznamu. */
    DLLElementPtr firstElement;
    /** Ukazatel na aktuální prvek seznamu. */
    DLLElementPtr activeElement;
    /** Ukazatel na posledni prvek seznamu. */
    DLLElementPtr lastElement;
} DLList;

void DLL_DeleteFirst( int num);

void DLL_Init(int num);

void DLL_Dispose(int num);

void DLL_InsertFirst(int num, char *data);

int GetNumberOfDigets();

void GetUniqueVarName();

void GetUniqueName();
/** stored while cond and normal condition */
static DLList *listCodeGen = NULL;

/** stored condition labels */
static DLList *listIfLabels = NULL;

/** stored while labels */
static DLList *listWhileLabels = NULL;

/** string name of function*/
static char *functionName = NULL;

/** int to get unique names of var, labels, function, ...*/
static int UniqueName = 0;

/** int to get unique names of var, labels, function, ...*/
static int UniqueVarName = 0;

/** bool if in if body*/
static bool Return = 0;

/** int if in if body*/
static int inIf = 0;

/** int if in While body*/
static int inWhile = 0;

/** String for global frame -> 0 */
static char *generatedString = NULL;

/** String for actual function -> 1 */
static char *inFunctionString = NULL;

/** String which will be printed -> 2 */
static char *allFunctionsString = NULL;

/** Condition/While condition asm code */
static char *whileIfString = NULL;

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
static enum type operator= NOT_DEFINED;

/**
 * @brief Function which append newStr to str.
 *
 * @param str 0 -> generatedString
              1 -> inFunctionString
              2 -> allFunctionsString
              3 -> whileIfString
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
 * @brief Same as threeAddress function but without calling remove functions at the end.
 *
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void threeAddressWithoutRemove(int frameStr, char *frame);

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

/**
 * @brief Function which converts int to float if needed.
 *
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void convertToSameType(int frameStr, char *frame);

/**
 * @brief Function which generates random string.
 *
 * @param dest destination where new string will be saved
 * @param length number of chars generated
 */
void randStr(char *dest, size_t length);

/**
 * @brief Creating instructions for dividing.
 *
 * @param frameStr  0 -> generatedString
                    1 -> inFunctionString
                    2 -> allFunctionsString
 * @param frame  GF/TF/LF
 */
void divIdiv(int frameStr, char *frame);

/*** End of code_generation.h ***/
#endif