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

    codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (token->t != L_CPAR)
    {                 // FUNCTION <function_call> : <type> {
        return false; // missing L_CPAR
    }

    codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (statement(token) == 0)
    {                 // FUNCTION <function_call> : <type> { <statement>
        return false; // invalid statement
    }
    // codeGeneration(token);
    dtorToken(token);
    token = getToken();
    if (token->t != R_CPAR)
    {                 // FUNCTION <function_call> : <type> { <statement> }
        return false; // missing R_CPAR
    }

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
        // codeGeneration(token);
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
    if (token->t == STRING || token->t == VAR_ID || token->t == FLOAT || token->t == INT)
    { // VAR_ID OR STRING OR INT/FLOAT
        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (params_n(token) == 1)
        { // (VAR_ID OR STRING OR INT/FLOAT) <params_n>
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
    else if (type(token) == 1)
    { // <type>

        // gets function which is currently being declared
        SymUnion *currentlyDeclared = peekCurrentlyDeclared();
        if (!currentlyDeclared->function)
        {
            FREE_EXIT(99, ERROR_99_INTERNAL_ERROR, "");
        }
        // adds param to function with its type
        SymFunction *currentlyDeclaredFunction = currentlyDeclared->function;
        SymFunctionParam *param = addSymFunctionParam(currentlyDeclaredFunction, token->t, token->val[0] == '?');
        printSymTable();
        // param with type added to function

        codeGeneration(token);
        dtorToken(token);

        token = getToken();
        // passed only if token-> == VAR_ID, possible error
        if (token->t == VAR_ID)
        {
            //<type> VAR_ID
            printf("TOKEN: %s\n", token->val);
            nameSymFunctionParam(currentlyDeclaredFunction, param, token->val);
            printSymTable();
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

int expression(Token *token)
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
        codeGeneration(exp->tokenArray[0]);

        return 1;
    }

    if (exp->arrayLen != 0)
    {
        int resultType;
        if (bottomUp(exp, &resultType))
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
            if (expression(token))
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
                            codeGeneration(token);
                            dtorToken(token);
                            token = getToken();
                            if (token->t == R_CPAR)
                            { // IF ( <expression> ) { <statement> }
                                // codeGeneration(token);
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
            // checks if function with same name exists
            SymFunction *function = getFunction(token->val);
            if (function)
            {
                FREE_EXIT(3, ERROR_3_FUNCTION_NOT_DEFINED_REDEFINED, function->name);
            }
            // adds function with name to symtable
            else
            {
                function = addSymFunction(token->val);
                pushCurrentlyDeclared(function, NULL);
                printSymTable();
            }
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
                    // function checked - OK
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
            if (expression(token) == 1)
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
                            // codeGeneration(token);
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
        return 2;
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

        codeGeneration(token);
        dtorToken(token);
        token = getToken();
        if (token->t == EQ)
        { // VAR_ID =
            codeGeneration(token);
            dtorToken(token);
            token = getToken();

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
                char *tmpStr = calloc(token->valLen + 1, sizeof(char));
                strcpy(tmpStr, token->val);
                codeGeneration(token);
                dtorToken(token);
                token = getToken();
                if (token->t == SEMICOL)
                { // VAR_ID = VAR_ID;

                    free(tmpStr);
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
                    if (expression(tmp) == 1)
                    { // VAR_ID = <expression>
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
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            else if (var_rule(token) == 1)
            { // VAR_ID = <var_rule>
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
                if (expression(token) == 1)
                { // VAR_ID = <expression>
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
            return 1;
        }
        else if (var_rule(token) == 1)
        { // RETURN <var_rule>

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
            {                          // RETURN <var_rule>;
                codeGeneration(token); // added
                dtorToken(tmp);
                return 1;
            }
            else
            {
                pushTokenToStdin(token);
                if (expression(tmp) == 1)
                { // RETURN <expression> in case that first token of expression is VAR_ID
                    token = getToken();
                    if (token->t == SEMICOL)
                    {
                        codeGeneration(token); // added
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
        }
        else if (var_rule(token) == 2)
        {
            codeGeneration(token);
            dtorToken(token);
            token = getToken();
            if (expression(token) == 1)
            { // RETURN <expression>
                // dtorToken(token);
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
            // codeGeneration(token);
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
            // codeGeneration(token);
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
    else
    { // epsilon
        if (token->t == R_CPAR)
        {
            ungetc('}', stdin);
        }
        else
        {
            exit(2);
        }
        return 2;
    }
}

int main()
{
    symTable = initSymTable();
    printSymTable();
    // addBuiltInToSymtable();
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
