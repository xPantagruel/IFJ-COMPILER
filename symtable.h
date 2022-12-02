#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"

#define SYMTABLE_SIZE 100

typedef enum
{
    CURRENTLY_DECLARED_FUNCTION,
    CURRENTLY_DECLARED_VARIABLE
} currentlyDeclaredType;

typedef struct symTable SymTable;
extern SymTable *symTable;

typedef struct symFunctionReturn
{
    enum type type;
    bool canBeNull;
} SymFunctionReturn;
typedef struct symFunctionParam
{
    char *name;
    enum type type;
    bool canBeNull;
} SymFunctionParam;
typedef struct symVariable
{
    char *name;
    enum type type;
} SymVariable;

typedef struct symFunction
{
    SymFunctionParam **params;
    char *name;
    int paramCount;
    SymFunctionReturn *returnType;
} SymFunction;

typedef struct symItem SymItem;

typedef struct symItem
{
    char *key;
    SymFunction *function;
    SymVariable *variable;
    SymItem *next;
} SymItem;

typedef struct currentlyDeclaredObject
{
    currentlyDeclaredType objectType;
    union
    {
        SymFunction *function;
        SymVariable *variable;
    };
} CurrentlyDeclaredObject;

typedef struct frame
{
    int size;
    SymItem **items;
    char *name;
} Frame;
typedef struct symTable
{
    Frame *topFrame;
    Frame *mainFrame;
    CurrentlyDeclaredObject **currentlyDeclared;
    int currentlyDeclaredCount;
} SymTable;

/**
 * @brief Initializes symTable including main frame
 *
 * @return pointer to initialized symTable
 */
SymTable *initSymTable();

/**
 * @brief Initilazizes a new frame and attaches it to symTable
 *
 * @param name name of the frame
 * @param symTable to attach frame to
 * @return pointer to initialized symTable
 */
Frame *initFrame(char *name);

/**
 * @brief Initilazizes a new symItem
 *
 * @param key item key
 * @return pointer to initialized symItem
 */
SymItem *initSymItem(char *key);

/**
 * @brief Initilazizes a new function
 *
 * @param name function name
 * @return pointer to initialized function
 */
SymFunction *initSymFunction(char *name);

/**
 * @brief Initilazizes a new variable
 *
 * @param name function name
 * @return pointer to initialized variable
 */
SymVariable *initSymVariable(char *name);

/**
 * @brief Adds symItem to top frame inside symTable
 *
 * @param key item key
 * @param variable pointer to variable to attach to the item
 * @param function pointer to function to attach to the item
 * @return pointer to added symItem
 */
SymItem *addSymItem(char *key, SymFunction *function, SymVariable *variable);

/**
 * @brief Adds function to top frame inside symTable
 *
 * @param key function name
 * @return pointer to added function or NULL if the item already exists
 */
SymFunction *addSymFunction(char *key);

/**
 * @brief Adds variable to top frame inside symTable, if the variable already exists returns it
 *
 * @param key function name
 * @return pointer to added/existing variable
 */
SymVariable *addSymVariable(char *key);

/**
 * @brief Add new param with its type to an existing function
 *
 * @param function to add param to
 * @param type of the param
 * @param canBeNull indicates if null can be passed as param
 * @return added param
 */
SymFunctionParam *addSymFunctionParam(SymFunction *function, enum type type, bool canBeNull);

/**
 * @brief Add new param with its type to an existing function
 *
 * @param function to add return to
 * @param type of the return
 * @param canBeNull indicates if null can be returned
 * @return added return
 */
SymFunctionReturn *addSymFunctionReturn(SymFunction *function, enum type type, bool canBeNull);

/**
 * @brief names function param
 *
 * @param param to name
 * @param name what to name param
 * @return pointer to initialized function
 */
void nameSymFunctionParam(SymFunction *function, SymFunctionParam *param, char *name);

/**
 * @brief Searches top frame inside symtable for item with key
 *
 * @param key searched key
 * @return pointer to item or null if item doesn't exits
 */
SymItem *getItem(char *key);

/**
 * @brief Searches top frame inside symtable for function with key
 *
 * @param key searched key
 * @return pointer to function or null if item doesn't exits
 */
SymFunction *getFunction(char *key);

/**
 * @brief Searches top frame inside symtable for variable with key
 *
 * @param key searched key
 * @return pointer to variable or null if item doesn't exits
 */
SymVariable *getVariable(char *key);

/**
 * @brief checks if function returs correct type
 *
 * @throw 2 when types doesn't match
 */
void checkReturnType(SymFunction *function, enum type type);

/**
 * @brief frees allocated resources of the function
 *
 * @param function to free
 */
void freeSymFunction(SymFunction *function);

/**
 * @brief frees allocated resources of the variable
 *
 * @param variable to free
 */
void freeSymVariable(SymVariable *variable);

/**
 * @brief frees allocated resources of the item
 *
 * @param item to free
 */
void freeSymItem(SymItem *item);

/**
 * @brief frees allocated resources of the frame
 *
 * @param frame to free
 */
void freeFrame(Frame *frame);

/**
 * @brief frees allocated resources of symTable
 *
 */
void freeSymTable();

/**
 * @brief pushes variable or function to currentlyDeclared array in symTable
 *
 * @param object var or function to push
 */
void pushCurrentlyDeclared(SymFunction *function, SymVariable *variable, currentlyDeclaredType objectType);

/**
 * @brief peeks variable or function from currentlyDeclared array in symTable
 *
 * @return var or function on top of currentlyDeclared stack
 */
CurrentlyDeclaredObject *peekCurrentlyDeclared();

/**
 * @brief peeks function from currentlyDeclared array in symTable
 *
 * @return function on top of currentlyDeclared stack or null when top not function
 */
SymFunction *peekCurrentlyDeclaredFunction();

/**
 * @brief peeks variable from currentlyDeclared array in symTable
 *
 * @return function on top of currentlyDeclared stack or null when top not variable
 */
SymVariable *peekCurrentlyDeclaredVariable();

/**
 * @brief pops variable or function from currentlyDeclared array in symTable
 *
 */
void popCurrentlyDeclared();

void printSymVariable(SymVariable *variable);

void printSymFunction(SymFunction *function);

void printSymTable();

#endif
