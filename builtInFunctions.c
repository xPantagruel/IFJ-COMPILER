#include "builtInFunctions.h"

void addBuiltInFunction(char *name, char *paramNames[], enum type paramTypes[], bool paramsCanBeNull[], int numberOfParams, enum type returnType, bool canReturnNull)
{
    SymFunction *function = addSymFunction(name);
    if (numberOfParams != DYNAMIC_NUMBER_OF_PARAMS)
    {
        for (int i = 0; i < numberOfParams; i++)
        {
            SymFunctionParam *param = addSymFunctionParam(function, paramTypes[i], paramsCanBeNull[i]);
            nameSymFunctionParam(function, param, paramNames[i]);
        }
    }
    function->paramCount = numberOfParams;
    addSymFunctionReturn(function, returnType, canReturnNull);
}

void addAllBuiltInFunctions()
{
    addBuiltInFunction("reads", NULL, NULL, NULL, 0, STRING, true);

    addBuiltInFunction("readi", NULL, NULL, NULL, 0, INT, true);

    addBuiltInFunction("readf", NULL, NULL, NULL, 0, FLOAT, true);

    addBuiltInFunction("write", NULL, NULL, NULL, DYNAMIC_NUMBER_OF_PARAMS, VOID, false);

    {
        char *paramNames[1] = {"term"};
        enum type paramTypes[1] = {DYNAMIC};
        bool paramCanBeNUll[1] = {true};
        addBuiltInFunction("floatval", paramNames, paramTypes, paramCanBeNUll, 1, FLOAT, false);
    }

    {
        char *paramNames[1] = {"term"};
        enum type paramTypes[1] = {DYNAMIC};
        bool paramCanBeNUll[1] = {true};
        addBuiltInFunction("intval", paramNames, paramTypes, paramCanBeNUll, 1, INT, false);
    }

    {
        char *paramNames[1] = {"term"};
        enum type paramTypes[1] = {STRING};
        bool paramCanBeNUll[1] = {true};
        addBuiltInFunction("strval", paramNames, paramTypes, paramCanBeNUll, 1, STRING, false);
    }

    {
        char *paramNames[1] = {"s"};
        enum type paramTypes[1] = {STRING};
        bool paramCanBeNUll[1] = {false};
        addBuiltInFunction("strlen", paramNames, paramTypes, paramCanBeNUll, 1, INT, false);
    }

    {
        char *paramNames[3] = {"s", "i", "j"};
        enum type paramTypes[3] = {STRING, INT, INT};
        bool paramCanBeNUll[3] = {false, false, false};
        addBuiltInFunction("substring", paramNames, paramTypes, paramCanBeNUll, 3, STRING, false);
    }

    {
        char *paramNames[1] = {"c"};
        enum type paramTypes[1] = {STRING};
        bool paramCanBeNUll[1] = {false};
        addBuiltInFunction("ord", paramNames, paramTypes, paramCanBeNUll, 1, INT, false);
    }

    {
        char *paramNames[1] = {"i"};
        enum type paramTypes[1] = {INT};
        bool paramCanBeNUll[1] = {false};
        addBuiltInFunction("chr", paramNames, paramTypes, paramCanBeNUll, 1, STRING, true);
    }
}