/**
 * @file parser.c
 *
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 *
 * @brief Implementation of syntax analysis of PHP like language.
 *
 * @date 2022-10-14
 */

#include "bottomUp.h"
#include "parser.h"
#include "symtable.h"
#include "code_generation.h"
#include "builtInFunctions.h"

Expression *initExpression()
{
    Expression *exp = calloc(1, sizeof(Expression));
    if (exp == NULL)
    { // malloc failed
        exit(99);
    }
    exp->arrayLen = 0;
    exp->tokenArray = NULL;
    return exp;
}

void dtorExpression(Expression *exp)
{
    (void)exp;
    // for (int i = 0; i < exp->arrayLen; i++)
    // {
    //     dtorToken(exp->tokenArray[i]);
    // }
    // free(exp);
}

void addTokenToExpression(Expression *exp, Token *token)
{
    exp->tokenArray = realloc(exp->tokenArray, (exp->arrayLen + 1) * sizeof(Token *));

    if (exp->tokenArray == NULL) // realloc failed
    {
        exit(99);
    }
    exp->tokenArray[exp->arrayLen] = token;
    exp->arrayLen++;
}

bool function_declaration(Token *token)
{
    if (iAmInConditionWhileFunRule)
    { // function declaration can't be in IF, WHILE or in other function declaration
        return false;
    }

    iAmInConditionWhileFunRule = 1;

    codeGeneration(token);
    dtorToken(token);
    token = getToken();

    if (function_call(token, true) == 0)
    {                 // FUNCTION <function_call>
        return false; // missing function name
    }

    codeGeneration(token);
    dtorToken(token);
    token = getToken();

    if (token->t != COLON)
    {                 // FUNCTION <function_call> :
        return false; // missing COLON
    }
    codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (type(token) != 1)
    {
        // FUNCTION <function_call> : <type>
        return false; // wrong type
    }

    // add return type to function

    SymFunction *function = peekCurrentlyDeclaredFunction();
    addSymFunctionReturn(function, token->t, token->val[0] == '?');
    // added return type to the function
    // function header done

    codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (token->t != L_CPAR)
    {                 // FUNCTION <function_call> : <type> {
        return false; // missing L_CPAR
    }

    // attach function frame
    Frame *frame = initFrame(function->name);
    symTable->topFrame = frame;
    // function frame attached to symtable

    // create local variables from params
    addFunctionLocalVariables(function);

    codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (statement(token) == 0)
    {                 // FUNCTION <function_call> : <type> { <statement>
        return false; // invalid statement
    }
    // //codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (token->t != R_CPAR)
    {                 // FUNCTION <function_call> : <type> { <statement> }
        return false; // missing R_CPAR
    }

    // remove local variables, delete currentFrame
    Frame *topFrame = symTable->topFrame;
    if (topFrame == symTable->mainFrame)
    {
        FREE_EXIT(99, ERROR_99_INTERNAL_ERROR, "");
    }

    if (function->returnType->type != VOID && !topFrame->returnFound)
    {
        FREE_EXIT(4, ERROR_4_FUNCTION_INCORRECT_CALL, function->name);
    }

    freeFrame(topFrame);
    // pop currently declared function
    popCurrentlyDeclared();
    // removed

    iAmInConditionWhileFunRule = 0; // I am out of function declaration
    return true;
}

bool prog(Token *token)
{
    if (token->t != EOF_T)
    { //<prog> -> EOF
        if (token->t == FUNCTION)
        { // FUNCTION
            if (!function_declaration(token))
            {
                return false;
            }
            else
            {
                codeGeneration(token);
            }
        }
        else if (statement(token) == 0)
        {                 //<prog> -> <statement>
            return false; // invalid statement
        }
        // //codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (prog(token))
        { // RECURSION
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true; // EOF
    }
}

int type(Token *token)
{
    if (token->t == FLOAT_TYPE)
    {
        return 1;
    }
    else if (token->t == INT_TYPE)
    {
        return 1;
    }
    else if (token->t == STRING_TYPE)
    {
        return 1;
    }
    else if (token->t == VOID)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

int params(Token *token, int paramIndex)
{
    if (token->t == STRING || token->t == VAR_ID || token->t == FLOAT || token->t == INT || token->t == NULL_KEYWORD)
    { // VAR_ID OR STRING OR INT/FLOAT

        SymFunction *function = peekCurrentlyDeclaredFunction();
        // check if param passed has correct type
        checkFunctionParam(function, token->val, token->t, paramIndex);
        paramIndex++;

        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (params_n(token) == 1)
        { // (VAR_ID OR STRING OR INT/FLOAT) <params_n>
            if (token->t == R_PAR)
            {
                exit(2);
            }
            if (params(token, paramIndex))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if (params_n(token) == 2)
        { // epsilon
            ungetc(')', stdin);
            checkFunctionParamCount(function, paramIndex);

            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (type(token) == 1)
    { // <type>

        // gets function which is currently being declared
        SymFunction *currentlyDeclaredFunction = peekCurrentlyDeclaredFunction();
        // adds param to function with its type
        SymFunctionParam *param = addSymFunctionParam(currentlyDeclaredFunction, token->t, token->val[0] == '?');
        // param with type added to function

        codeGeneration(token);
        dtorToken(token);

        token = getToken();
        // passed only if token-> == VAR_ID, possible error
        if (token->t == VAR_ID)
        {
            //<type> VAR_ID
            // names param of the function
            nameSymFunctionParam(currentlyDeclaredFunction, param, token->val);
            // param named

            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (params_n(token) == 1)
            { // <type> VAR_ID OR <type> STRING OR <type> INT/FLOAT <params_n>
                if (params(token, ++paramIndex))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else if (params_n(token) == 2)
            { // epsilon
                ungetc(')', stdin);
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    { // epsilon

        SymFunction *function = peekCurrentlyDeclaredFunction();
        checkFunctionParamCount(function, 0);
        ungetc(')', stdin);
        return 2;
    }
}

int params_n(Token *token)
{
    if (token->t == R_PAR)
    { // next token is )
        return 2;
    }
    else if (token->t == COMMA)
    {
        codeGeneration(token);
        dtorToken(token);
        token = getToken(); // token is set to the next one
        return 1;
    }
    else
    {
        return 0;
    }
}

int expression(Token *token, enum type *returnType)
{
    Expression *exp = initExpression();
    int count = 0;

    while (token->t != SEMICOL && (token->t != R_PAR || count != 0))
    { // getting all tokens in expression
        if (token->t == L_PAR)
            count++;
        if (token->t == R_PAR)
        {
            count--;
        }
        if (count < 0)
        {
            // error
        }

        addTokenToExpression(exp, token);
        token = getToken();
    }

    if (token->t == SEMICOL)
    {
        ungetc(';', stdin);
    }
    else
    {
        ungetc(')', stdin);
    }

    if (exp->arrayLen == 1)
    {
        if (exp->tokenArray[0]->val == NULL)
        {
            exp->tokenArray[0]->t = INT;
            addCharToToken('0', exp->tokenArray[0]);
            codeGeneration(exp->tokenArray[0]);
        }
        else
        {
            codeGeneration(exp->tokenArray[0]);
            if (returnType)
            {
                *returnType = exp->tokenArray[0]->t;
            }
        }

        return 1;
    }

    if (exp->arrayLen != 0)
    {

        // for (int i = 0; i < exp->arrayLen; i++)
        // {
        //     printf("%s exp\n", exp->tokenArray[i]->val);
        // }

        if (bottomUp(exp, returnType))
        {

            dtorExpression(exp);
            return 1;
        }
        else
        {
            dtorExpression(exp);
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int condition(Token *token)
{
    if (token->t == IF)
    { // IF
        iAmInConditionWhileFunRule = 1;
        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (token->t == L_PAR)
        {                          // IF (
            codeGeneration(token); // todo mozno tu dtor
            token = getToken();
            if (expression(token, NULL))
            { // IF ( <expression>
                // dtorToken(token);
                token = getToken();
                if (token->t == R_PAR)
                { // IF ( <expression> )
                    codeGeneration(token);
                    dtorToken(token);
                    token = getToken();
                    if (token->t == L_CPAR)
                    { // IF ( <expression> ) {
                        codeGeneration(token);
                        dtorToken(token);
                        token = getToken();
                        if (statement(token))
                        { // IF ( <expression> ) { <statement>
                            // codeGeneration(token);
                            dtorToken(token);
                            token = getToken();
                            if (token->t == R_CPAR)
                            { // IF ( <expression> ) { <statement> }
                                codeGeneration(token);
                                dtorToken(token);
                                token = getToken();
                                if (token->t == ELSE)
                                { // IF ( <expression> ) { <statement> } ELSE
                                    codeGeneration(token);
                                    dtorToken(token);
                                    token = getToken();
                                    if (token->t == L_CPAR)
                                    { // IF ( <expression> ) { <statement> } ELSE {
                                        codeGeneration(token);
                                        dtorToken(token);
                                        token = getToken();
                                        if (statement(token))
                                        { // IF ( <expression> ) { <statement> } ELSE { <statement>
                                            codeGeneration(token);
                                            dtorToken(token);
                                            token = getToken();
                                            if (token->t == R_CPAR)
                                            { // IF ( <expression> ) { <statement> } ELSE { <statement> }
                                                codeGeneration(token);
                                                iAmInConditionWhileFunRule = 0;
                                                return 1;
                                            }
                                            else
                                            {
                                                return 0;
                                            }
                                        }
                                        else
                                        {
                                            return 0;
                                        }
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 2;
    }
}

int function_call(Token *token, bool isDeclaration)
{
    if (token->t == ID)
    { // ID

        if (isDeclaration)
        {
            // declares function with its name
            SymFunction *function = declareFunction(token->val);
            pushCurrentlyDeclared(function, NULL, DECLARED_FUNCTION);
        }
        else
        {

            // checks if function exists
            SymFunction *function = checkFunctionCall(token->val);
            pushCurrentlyDeclared(function, NULL, DECLARED_FUNCTION);
        }

        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (token->t == L_PAR)
        { // ID (

            codeGeneration(token);
            dtorToken(token);
            token = getToken();

            if (params(token, 0))
            { // ID ( <params>

                // dtorToken(token);
                token = getToken();

                if (token->t == R_PAR)
                { // ID ( <params> )
                    if (!isDeclaration)
                    {
                        popCurrentlyDeclared();
                    }

                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 2;
    }
}

int while_rule(Token *token)
{
    if (token->t == WHILE)
    { // while
        iAmInConditionWhileFunRule = 1;
        codeGeneration(token);
        token = getToken(); // mozno tu free??
        if (token->t == L_PAR)
        { // while (
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (expression(token, NULL) == 1)
            { // while ( <expression>
                // dtorToken(token);
                token = getToken();
                if (token->t == R_PAR)
                { // while ( <expression> )
                    codeGeneration(token);
                    dtorToken(token);
                    token = getToken();
                    if (token->t == L_CPAR)
                    { // while ( <expression> ) {

                        codeGeneration(token);
                        dtorToken(token);
                        token = getToken();
                        if (statement(token))
                        { // while ( <expression> ) { <statement>
                            // //codeGeneration(token);
                            dtorToken(token);
                            token = getToken();
                            if (token->t == R_CPAR)
                            { // while ( <expression> ) { <statement> }
                                codeGeneration(token);
                                iAmInConditionWhileFunRule = 0;
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 2;
    }
}

void pushTokenToStdin(Token *token)
{
    for (unsigned int i = 0; i < strlen(token->val); i++)
    {
        ungetc(token->val[strlen(token->val) - i - 1], stdin);
    }

    dtorToken(token);
}

int var_rule(Token *token)
{
    if (token->t == VAR_ID || token->t == STRING || token->t == INT || token->t == FLOAT || token->t == NULL_KEYWORD)
    { // <var_rule> (VAR_ID/STRING/INT/FLOAT/NULL)

        if (token->t == VAR_ID)
        {
            SymVariable *assignedVariable = getVariable(token->val);
            if (!assignedVariable)
            {
                // assigned variable doesn't exist
                FREE_EXIT(5, ERROR_5_VARIABLE_NOT_DEFINED, token->val);
            }
        }

        return 2;
    }
    else if (token->t == ID)
    { // <var_rule> (<function_call> (ID))

        if (function_call(token, false) == 1)
        { // <var_rule> (<function_call>)

            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

int statement(Token *token)
{
    if (token->t == FUNCTION)
    {
        if (function_declaration(token))
        {
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (statement(token))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (token->t == VAR_ID)
    { // VAR_ID

        // add variable
        SymVariable *variable = addSymVariable(token->val);
        pushCurrentlyDeclared(NULL, variable, DECLARED_VARIABLE);

        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (token->t == EQ)
        { // VAR_ID =
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            char *tmpString = calloc(strlen(token->val) + 1, 1);
            strcpy(tmpString, token->val);
            if (token->t == VAR_ID)
            { // VAR_ID = VAR_ID

                // if token == var_id -> can be <expression>
                // storing previous token
                Token *tmp = tokenInit();
                addRowToToken(token->row, tmp);
                addTypeToToken(token->t, tmp);
                for (unsigned long i = 0; i < strlen(token->val); i++)
                {
                    addCharToToken(token->val[i], tmp);
                }

                codeGeneration(token);
                dtorToken(token);
                token = getToken();

                if (token->t == SEMICOL)
                { // VAR_ID = VAR_ID;

                    // check if variable being assigned exists
                    SymVariable *assignedVariable = getVariable(tmpString);
                    if (!assignedVariable)
                    {
                        // assigned variable doesn't exist
                        FREE_EXIT(5, ERROR_5_VARIABLE_NOT_DEFINED, tmpString);
                    }
                    // assigned variable exists, assign it's type to variable
                    variable->type = assignedVariable->type;
                    free(tmpString);
                    popCurrentlyDeclared();
                    // variable added

                    dtorToken(tmp);
                    codeGeneration(token);
                    dtorToken(token);
                    token = getToken();
                    if (statement(token))
                    { // VAR_ID = VAR_ID; <statement>

                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    pushTokenToStdin(token);
                    int returnType;
                    if (expression(tmp, &returnType) == 1)
                    { // VAR_ID = <expression>

                        // add expression result as variable type
                        variable->type = returnType;
                        popCurrentlyDeclared();
                        free(tmpString);
                        token = getToken();
                        if (token->t == SEMICOL)
                        { // VAR_ID = <expression> ;
                            codeGeneration(token);
                            dtorToken(token);
                            token = getToken();
                            if (statement(token))
                            { // VAR_ID = <expression>; <statement>
                                return 1;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            free(tmpString);
                            return 0;
                        }
                    }
                    else
                    {
                        free(tmpString);
                        return 0;
                    }
                }
            }
            else if (var_rule(token) == 1)
            { // VAR_ID = <var_rule>

                SymFunction *function = getFunction(tmpString);
                variable->type = function->returnType->type;
                // pop after var = function_call
                popCurrentlyDeclared();
                free(tmpString);
                codeGeneration(token);
                dtorToken(token);
                token = getToken();
                if (token->t == SEMICOL)
                { // VAR_ID = <var_rule>;
                    codeGeneration(token);
                    dtorToken(token);
                    token = getToken();
                    if (statement(token))
                    { // VAR_ID = <var_rule>; <statement>
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else if (var_rule(token) == 2)
            {
                int returnType;
                if (expression(token, &returnType) == 1)
                { // VAR_ID = <expression>

                    // add expression result as variable type
                    variable->type = returnType;
                    popCurrentlyDeclared();
                    free(tmpString);
                    // dtorToken(token);
                    token = getToken();
                    if (token->t == SEMICOL)
                    { // VAR_ID = <expression>;
                        codeGeneration(token);
                        dtorToken(token);
                        token = getToken();
                        if (statement(token))
                        { // VAR_ID = <expression>; <statement>
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                { // no more options -> error
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (token->t == RETURN)
    { // RETURN
        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (token->t == SEMICOL)
        { // RETURN ;

            // check if return type is void

            if (symTable->topFrame != symTable->mainFrame)
            {
                // gets function which is currently being declared
                SymFunction *function = peekCurrentlyDeclaredFunction();
                if (symTable->topFrame->returnFound)
                {
                    FREE_EXIT(6, ERROR_6_FUNCTION_INCORRECT_RETURN, function->name);
                }

                // check if return type of that function is void
                checkReturnType(function, VOID);
                symTable->topFrame->returnFound = true;
            }
            else
            {
                if (symTable->topFrame->returnFound)
                {
                    FREE_EXIT(6, ERROR_6_FUNCTION_INCORRECT_RETURN, "");
                }
            }

            codeGeneration(token);
            return 1;
        }

        else if (var_rule(token) == 1)
        { // RETURN <var_rule>
            return 0;
        }
        else if (var_rule(token) == 2)
        {
            // //codeGeneration(token);
            //  dtorToken(token);
            // token = getToken();
            int returnType;
            if (expression(token, &returnType) == 1)
            { // RETURN <expression>

                if (symTable->topFrame != symTable->mainFrame)
                {
                    SymFunction *function = peekCurrentlyDeclaredFunction();

                    // check if function hasn't been returned already
                    if (symTable->topFrame->returnFound)
                    {
                        FREE_EXIT(6, ERROR_6_FUNCTION_INCORRECT_RETURN, function->name);
                    }

                    // check if return type of that function is void
                    checkReturnType(function, returnType);
                    symTable->topFrame->returnFound = true;
                }
                else
                {
                    if (symTable->topFrame->returnFound)
                    {
                        FREE_EXIT(6, ERROR_6_FUNCTION_INCORRECT_RETURN, "");
                    }
                }

                // dtorToken(token); //todo tu mozno leak
                token = getToken();
                if (token->t == SEMICOL)
                {                          // RETURN <expression>;
                    codeGeneration(token); // mozno chyba dtor added
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (token->t == IF)
    { // <condtion> (IF)
        if (condition(token) == 1)
        { // <condition>
            // //codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (statement(token))
            { // <condition> <statement>
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (token->t == WHILE)
    { //<while> (WHILE)
        if (while_rule(token) == 1)
        { // <while>
            // //codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (statement(token))
            { // <while> <statement>
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (token->t == ID)
    { // <function_call> (ID)
        if (function_call(token, false) == 1)
        { // <function_call>
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (token->t == SEMICOL)
            { // <function_call> ;
                codeGeneration(token);
                dtorToken(token);
                token = getToken();
                if (statement(token))
                { // <function_call> ; <statement>

                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    // <statement> ; <statement>
    else if ((token->t == SLASH) || (COMMA <= token->t && token->t <= MUL) || (ELSE <= token->t && token->t <= FLOAT_TYPE) || (token->t == INT_TYPE) || (token->t == RETURN) || (token->t == STRING_TYPE) || (token->t == VOID) || (EQ <= token->t && token->t <= NOT_EQ) || (token->t == COLON))
    {
        return 0;
    }
    else if ((token->t == NULL_KEYWORD) || (token->t == INT) || (token->t == FLOAT) || (token->t == STRING) || (token->t == SEMICOL) || (token->t == EOF_T))
    {
        if (token->t == EOF_T && iAmInConditionWhileFunRule)
        {
            exit(2);
        }

        if (token->t == SEMICOL)
        {
            codeGeneration(token);
        }
        else
        {
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (token->t != SEMICOL)
            {
                return 0;
            }
        }

        dtorToken(token);
        token = getToken();
        if (statement(token) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    { // epsilon
        if (token->t == R_CPAR)
        {
            ungetc('}', stdin);
        }
        return 2;
    }
}

int main()
{
    symTable = initSymTable();
    addAllBuiltInFunctions();
    //  Example how parser can be called.
    DLL_Init(0);
    DLL_Init(1);
    DLL_Init(2);

    Token *token = getToken();
    if (prog(token))
    {
        codeGeneration(token);
        dtorToken(token);

        if (generatedString != NULL)
        {
            free(generatedString);
        }
        return 0; // exit code 0
    }
    else
    {
        // dtorToken(token);
        if (generatedString != NULL)
        {
            free(generatedString);
        }
        exit(2);
    }
}

/*** End of parser.c ***/
