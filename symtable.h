#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"

#define SYMTABLE_SIZE 100

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
    bool canBeNull;
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

typedef union symTableUnion
{
    SymFunction *function;
    SymVariable *variable;
} SymTableUnion;

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
    SymTableUnion *currentlyDeclared;
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
 * @brief Add new param to an existing function
 *
 * @param function to add param to
 * @param name of the param
 * @return pointer to initialized function
 */
SymFunctionParam *addSymFunctionParam(SymFunction *function, char *name);

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
SymFunction *getVariable(char *key);

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

#endif
