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

/** Element of doubly linked list */
typedef struct DLLElement
{
    /** stored data */
    char *data;
    /** pointer to previous element */
    struct DLLElement *previousElement;
    /** pointer to next element */
    struct DLLElement *nextElement;
} * DLLElementPtr;

/** Doubly linked list */
typedef struct
{
    /** Pointer to first element */
    DLLElementPtr firstElement;
    /** Pointer to active element */
    DLLElementPtr activeElement;
    /** Pointer to last element */
    DLLElementPtr lastElement;
} DLList;

/** auxiliary variable to know if we are after case ELSE */
static bool afterElse=false;

/** temporary variable used for storing the first element of a list */
static DLLElementPtr listTmp = NULL;

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

/** float = 0, int = 1*/
static int floatIntOperator = 0;

/** auxiliary variable for storing previous type of token */
static enum type previousTokenType = NOT_DEFINED;

/** bool if in if body*/
static bool Return = 0;

/** if we created return code this variable will be 1 otherwise 0 */
static int returnedToStack = 0;

/** inIf == 1 if we are in IF */
static int inIf = 0;

/** variable to know if declarated or build in function was called */
static int buildInCalled = 0;

/** counter of params in function call */
static int functionCallParamsCounter = 0;

/** auxiliary variable to know if we are in function declaration and function label was created */
static int functionLabelCreated = 0;

/** temporary variable to store if and while labels */
static char TmpWhileAndIf[512];

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

/** Auxiliary variable to know if we are in function call */
static int IAmInFunctionCall = 0;

/** Auxiliary variable to know if we are in function declaration */
static int IAmInFunctionDeclaration = 0;

/** Auxiliary variable to know if we are in function */
static int IAmInFunction = 0;

/** auxiliary variable to know if we are calling function from global frame (because sometimes LF is set but we are still in GF - LF set after ID) */
static int callingFromGF = 0;

/** Counter of { and } */
static int cparCounter = 0;

/** Counter of ( */
static int lparCounter = 0;

/** Number of items in storage */
static int storageLen = 0;

/** if '=' was found -> 1 otherwise 0 */
static int eqSymbolFound = 0;

/** Storage for expressions (var1 = var2 + 2) */
static char **storage = NULL;

/** storage for build in functions params */
static char **buildInFunctionsParams = NULL;

/** Actual operator in expression */
static enum type operator= NOT_DEFINED;

/**
* @brief Function which adds LF or GF to final string.
*
* @param frameStr number of string to which LF or GF will be added
* @param IAmInFunction to know if we are in function declaration
*/
void AddLForFG(int frameStr, int IAmInFunction);

/**
* @brief Deleting first element in linked list.
*
* @param num id of doubly linked list from which we want to delete element
*/
void DLL_DeleteFirst(int num);

/**
* @brief Init of doubly linked list.
*
* @param num id of doubly linked list which we want to initializate.
*/
void DLL_Init(int num);

/**
* @brief Clearing of doubly linked list.
*
* @param num id of doubly linked list which we want to clear.
*/
void DLL_Dispose(int num);

/**
* @brief Inserting the first element in linked list.
*
* @param num id of doubly linked list to which we want to insert element
*/
void DLL_InsertFirst(int num, char *data);

/**
 *@brief Function to get numer of digets of UniqueName for purpose of malloc
 */
int GetNumberOfDigets();

/**
 *@brief Function add in UniqueVarName + 1 its for purpose of not having same name of variable
 */
void GetUniqueVarName();

/**
 *@brief Function add in UniqueName + 1 its for purpose of not having same name of LABEL
 */
void GetUniqueName();

/** 
* @brief Setting floatIntOperator variable to know if we want float or int.
*/
void setFloatIntOperatorVariable();

/** 
* @brief Function which append pushing zero to string.
*
* @param frame id of string where PUSH int@0 will be added
*/
void pushZero(int frame);

/**
* @brief Function which append calling label to string.
*
* @param frame id of string where call label will be created
*/
void createCallLabel(int frame);

/**
 * @brief Function for creating assembly return code.
 *
 * @param frame number of string to which params will be appended
 * @param frameStr LF/GF
 */
void createReturnCode(int frame, char *frameStr);

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
 * @brief Function for creating assembly while code after R_CPAR.
 *
 * @param frame number of string to which params will be appended
 */
void caseRcparCreateWhileCode(int frame);

/**
 * @brief Function for creating assembly while code after WHILE.
 *
 * @param frame number of string to which params will be appended
 */
void caseWhileCode(int frame);

/**
 * @brief Function for creating assembly if code after IF.
 *
 * @param frame number of string to which params will be appended
 */
void caseIfCreateIfCode(int frame);

/**
 * @brief Function for creating assembly else code after ELSE.
 *
 * @param frame number of string to which params will be appended
 */
void caseElseCreateElseCode(int frame);

/**
 * @brief Function for creating assembly if/else code after R_CPAR.
 *
 * @param frame number of string to which params will be appended
 */
void caseRcparCreateIfElseCode(int frame);

/**
 * @brief Function for appending operator to string.
 *
 * @param frame number of string to which operator will be appended
 */
void checkOperator(int frame);

/**
 * @brief "Main" function in code_generation.
 *
 * @param token token
 */
void codeGeneration(Token *token);

/**
 * @brief Function for creating assembly return code when concatenating strings.
 */
void returnConcat();

/**
 * @brief Function which we use to store values to storage.
 *
 * @param val value which will be stored to storage.
 */
void store(char *val);

/**
 * @brief Function which we use to store build in functions params to storage.
 *
 * @param val value which will be stored to build-in params storage.
 */
void storeBuildInParams(char *val);

/**
 * @brief Function which append assembly code for pushing params from build in functions.
 *
 * @param frame number of string to which params will be appended
 * @param frameStr LF/GF
 */
void writeAndFreeBuildInParams(int frame, char *frameStr);

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

/**
 *@brief Builtin function for reading string input from stdin
 */
void READS();

/**
 *@brief Builtin function for reading int input from stdin
 */
void READI();

/**
 *@brief Builtin function for reading float input from stdin
 */
void READF();

/**
 *@brief Builtin function for printing params on stdout
 */
void WRITE();

/**
 *@brief Builtin function for converting parameter to float
 */
void FLOATVAL();

/**
 *@brief Builtin function for converting parameter to int
 */
void INTVAL();

/**
 *@brief Builtin function for converting parameter to string
 */
void STRVAL();

/**
 *@brief Builtin function return length of parameter
 */
void STRLEN();

/**
 *@brief Builtin function for getting substring of parameter
 */
void SUBSTRING();

/**
 *@brief Builtin function returns the ordinal (ASCII) value of the first character in the string  
 */
void ORD();

/**
 *@brief Builtin function Returns a single character string with the character whose
 *       The ASCII code is specified by the parameter 𝑖.
 */
void CHR();


#endif

/*** End of code_generation.h ***/
