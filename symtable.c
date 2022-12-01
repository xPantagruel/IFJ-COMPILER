
#include "symtable.h"

SymTable *symTable;

size_t htab_hash_function(char *str)
{
    uint32_t h = 0;
    const unsigned char *p;
    for (p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}

SymTable *initSymTable()
{
    Frame *frame = initFrame("main");
    SymTable *symTable = calloc(1, sizeof(SymTable));
    if (!symTable)
    {
        exit(99);
    }
    symTable->mainFrame = frame;
    symTable->topFrame = symTable->mainFrame;
    symTable->currentlyDeclaredCount = 0;
    return symTable;
}

Frame *initFrame(char *name)
{
    Frame *frame = calloc(1, sizeof(Frame));
    if (!frame)
    {
        exit(99);
    }
    frame->name = calloc(strlen(name) + 1, 1);
    if (!frame->name)
    {
        exit(99);
    }
    strcpy(frame->name, name);
    frame->size = SYMTABLE_SIZE;
    frame->items = calloc(SYMTABLE_SIZE, sizeof(SymItem));
    return frame;
}

SymItem *initSymItem(char *key)
{
    SymItem *item = calloc(1, sizeof(SymItem));
    if (!item)
    {
        exit(99);
    }
    item->key = calloc(strlen(key) + 1, 1);
    if (!item->key)
    {
        exit(99);
    }
    strcpy(item->key, key);
    return item;
}

SymFunction *initSymFunction(char *name)
{
    SymFunction *function = calloc(1, sizeof(SymFunction));
    if (!function)
    {
        exit(99);
    }
    function->name = calloc(strlen(name) + 1, 1);
    if (!function->name)
    {
        exit(99);
    }
    strcpy(function->name, name);
    return function;
}

SymVariable *initSymVariable(char *name)
{
    SymVariable *variable = calloc(1, sizeof(SymVariable));
    if (!variable)
    {
        exit(99);
    }
    variable->name = calloc(strlen(name) + 1, 1);
    if (!variable->name)
    {
        exit(99);
    }
    strcpy(variable->name, name);
    return variable;
}

SymItem *addSymItem(char *key, SymFunction *function, SymVariable *variable)
{
    SymItem *item = initSymItem(key);
    item->function = function;
    item->variable = variable;

    int index = htab_hash_function(key) % SYMTABLE_SIZE;

    SymItem *tmp = symTable->topFrame->items[index];
    if (!tmp)
    {
        symTable->topFrame->items[index] = item;
    }
    else
    {
        while (tmp)
        {
            if (!tmp->next)
            {
                tmp->next = item;
                break;
            }
            tmp = tmp->next;
        }
    }
    return item;
}

SymFunction *addSymFunction(char *key)
{
    SymItem *item = getItem(key);

    if (item)
    {
        if (item->function)
        {
            return NULL;
        }
        SymFunction *function = initSymFunction(key);
        item->function = function;
        return item->function;
    }
    else
    {
        SymFunction *function = initSymFunction(key);
        item = addSymItem(key, function, NULL);
        return item->function;
    }
}

SymVariable *addSymVariable(char *key)
{
    SymItem *item = getItem(key);

    if (item)
    {
        if (item->variable)
        {
            return item->variable;
        }

        SymVariable *variable = initSymVariable(key);
        item->variable = variable;
        return item->variable;
    }
    else
    {
        SymVariable *variable = initSymVariable(key);
        item = addSymItem(key, NULL, variable);
        return item->variable;
    }
}

SymFunctionParam *addSymFunctionParam(SymFunction *function, enum type type, bool canBeNull)
{
    if (function)
    {
        SymFunctionParam *param = calloc(1, sizeof(SymFunctionParam));
        if (!param)
        {
            exit(99);
        }
        switch (type)
        {
        case INT_TYPE:
            param->type = INT;
            break;
        case FLOAT_TYPE:
            param->type = FLOAT;
            break;
        case STRING_TYPE:
            param->type = STRING;
            break;
        default:
            break;
        }
        param->name = NULL;
        param->canBeNull = canBeNull;
        function->params = realloc(function->params, (function->paramCount + 1) * sizeof(SymFunctionParam *));
        function->params[function->paramCount] = param;
        function->paramCount++;
        return param;
    }
    return NULL;
}

void nameSymFunctionParam(SymFunction *function, SymFunctionParam *param, char *name)
{
    for (int i = 0; i < function->paramCount; i++)
    {
        if (function->params[i]->name)
        {
            if (strcmp(function->params[i]->name, name) == 0)
            {
                exit(8);
            }
        }
    }

    param->name = calloc(strlen(name) + 1, 1);
    strcpy(param->name, name);
}

SymItem *getItem(char *key)
{
    int index = htab_hash_function(key) % symTable->topFrame->size;

    SymItem *tmp = symTable->topFrame->items[index];

    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

SymFunction *getFunction(char *key)
{
    SymItem *item = getItem(key);
    if (!item)
    {
        return NULL;
    }

    return item->function;
}

SymVariable *getVariable(char *key)
{
    SymItem *item = getItem(key);
    if (!item)
    {
        return NULL;
    }

    return item->variable;
}

void freeSymFunction(SymFunction *function)
{
    if (function)
    {
        free(function->name);
        for (int i = 0; i < function->paramCount; i++)
        {
            free(function->params[i]->name);
            free(function->params[i]);
        }
        if (function->returnType)
        {
            free(function->returnType);
        }
    }
}

void freeSymVariable(SymVariable *variable)
{
    if (variable)
    {
        free(variable->name);
        free(variable);
    }
}

void freeSymItem(SymItem *item)
{
    if (item)
    {
        freeSymFunction(item->function);
        item->function = NULL;
        freeSymVariable(item->variable);
        item->variable = NULL;
        free(item);
    }
}

void freeFrame(Frame *frame)
{
    if (frame)
    {
        for (int i = 0; i < frame->size; i++)
        {
            SymItem *item = frame->items[i];
            while (item)
            {
                SymItem *tmp = item->next;
                freeSymItem(item);
                item = tmp;
            }
            frame->items[i] = NULL;
        }

        free(frame->name);
        free(frame);
        symTable->topFrame = symTable->mainFrame;
    }
}

void freeSymTable()
{
    if (symTable->mainFrame == symTable->topFrame)
    {
        freeFrame(symTable->mainFrame);
    }
    else
    {
        freeFrame(symTable->topFrame);
        freeFrame(symTable->mainFrame);
    }
    free(symTable);
}

void pushCurrentlyDeclared(SymFunction *function, SymVariable *variable)
{
    SymUnion *uni = calloc(1, sizeof(SymUnion));
    if (function)
    {

        symTable->currentlyDeclared = realloc(symTable->currentlyDeclared, (symTable->currentlyDeclaredCount + 1) * sizeof(SymUnion *));
        symTable->currentlyDeclared[symTable->currentlyDeclaredCount] = uni;
        symTable->currentlyDeclared[symTable->currentlyDeclaredCount]->function = function;
    }
    else if (variable)
    {
        symTable->currentlyDeclared = realloc(symTable->currentlyDeclared, (symTable->currentlyDeclaredCount + 1) * sizeof(SymUnion));
        symTable->currentlyDeclared[symTable->currentlyDeclaredCount] = uni;
        symTable->currentlyDeclared[symTable->currentlyDeclaredCount]->variable = variable;
    }
    else
    {
        exit(99);
    }

    symTable->currentlyDeclaredCount++;
}

SymUnion *peekCurrentlyDeclared()
{
    if (symTable->currentlyDeclaredCount)
    {
        return symTable->currentlyDeclared[symTable->currentlyDeclaredCount - 1];
    }
    return NULL;
}

void popCurrentlyDeclared()
{
    if (symTable->currentlyDeclaredCount)
    {
        symTable->currentlyDeclared = realloc(symTable->currentlyDeclared, (symTable->currentlyDeclaredCount - 1) * sizeof(SymUnion));
    }
    symTable->currentlyDeclaredCount--;
}

void printSymFunction(SymFunction *function)
{
    printf("-----\n");
    printf("function: %s\n", function->name);
    if (function->returnType)
    {
        printf("type: %d\n", function->returnType->type);

        printf("canReturnNull: %d\n", function->returnType->canBeNull);
    }

    if (function->paramCount > 0)
    {
        printf("params:\n");
    }

    for (int i = 0; i < function->paramCount; i++)
    {
        if (function->params[i]->name)
        {
            printf("\t[%d]: %s\n", i, function->params[i]->name);
        }

        printf("\ttype: %d\n", function->params[i]->type);
        printf("\tcanBeNull?: %d\n", function->params[i]->canBeNull);
        printf("\n");
    }

    printf("-----\n");
}

void printSymVariable(SymVariable *variable)
{
    printf("-----\n");
    printf("variable: %s\n", variable->name);
    printf("type: %d\n", variable->type);
    printf("canBeNull?:%d\n", variable->canBeNull);
    printf("-----\n");
}

void printSymTable()
{
    printf("--CURRENTLY DECLARED--\n");
    for (int i = 0; i < symTable->currentlyDeclaredCount; i++)
    {
        SymUnion *item = symTable->currentlyDeclared[i];
        if (item->function)
        {
            printSymFunction(item->function);
        }
        else if (item->variable)
        {
            printSymVariable(item->variable);
        }
        else
        {
            printf("none\n");
        }
    }

    printf("--MAIN FRAME--\n");
    for (int i = 0; i < symTable->mainFrame->size; i++)
    {
        SymItem *tmp = symTable->mainFrame->items[i];
        while (tmp)
        {
            if (tmp->function)
                printSymFunction(tmp->function);
            if (tmp->variable)
                printSymVariable(tmp->variable);
            tmp = tmp->next;
        }
    }
    if (symTable->topFrame != symTable->mainFrame)
    {
        printf("--TOP FRAME--\n");
        for (int i = 0; i < symTable->topFrame->size; i++)
        {
            SymItem *tmp = symTable->topFrame->items[i];
            while (tmp)
            {
                if (tmp->function)
                    printSymFunction(tmp->function);
                if (tmp->variable)
                    printSymVariable(tmp->variable);
                tmp = tmp->next;
            }
        }
    }
}
