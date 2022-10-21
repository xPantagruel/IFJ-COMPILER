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
{ // TODO TU Chyba free moc vela
    for (int i = 0; i < exp->arrayLen; i++)
    {
        printf("%s\n", exp->tokenArray[i]->val); // uz to bude free
        // dtorToken(exp->tokenArray[i]);
    }
    // free(exp);
}

void addTokenToExpression(Expression *exp, Token *token) // todo mozno tu najskor malloc
{
    exp->tokenArray = realloc(exp->tokenArray, (exp->arrayLen + 1) * sizeof(Token *));
    if (exp->tokenArray == NULL)
    {
        exit(99);
    }
    exp->tokenArray[exp->arrayLen] = token;
    exp->arrayLen++;
}

bool prog(Token *token)
{
    if (token->t != EOF_T)
    { //<prog> -> EOF
        if (token->t == FUNCTION)
        { // FUNCTION
            dtorToken(token);
            token = getToken();
            if (function_call(token) == 0)
            {          // FUNCTION <function_call>
                false; // missing function name
            }
            dtorToken(token);
            token = getToken();
            if (token->t != COLON)
            {          // FUNCTION <function_call> :
                false; // missing COLON
            }
            dtorToken(token);
            token = getToken();
            if (type(token) != 1)
            {          // FUNCTION <function_call> : <type>
                false; // wrong type
            }
            dtorToken(token);
            token = getToken();
            if (token->t != L_CPAR)
            {          // FUNCTION <function_call> : <type> {
                false; // missing L_CPAR
            }
            dtorToken(token);
            token = getToken();
            if (statement(token) == 0)
            {          // FUNCTION <function_call> : <type> { <statement>
                false; // invalid statement
            }
            dtorToken(token);
            token = getToken();
            if (token->t != R_CPAR)
            {          // FUNCTION <function_call> : <type> { <statement> }
                false; // missing R_CPAR
            }
        }
        else if (statement(token) == 0)
        {          //<prog> -> <statement>
            false; // invalid statement
        }
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
    else
    {
        return 2;
    }
}

int params(Token *token)
{
    if (type(token) == 1)
    { // <type>
        dtorToken(token);
        token = getToken();
        if (token->t == VAR_ID)
        { //<type> VAR_ID
            dtorToken(token);
            token = getToken();
            if (params_n(token) == 1)
            {
                if (params(token))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else if (params_n(token) == 2)
            {
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
    {
        ungetc(')', stdin);
        return 2;
    }
}

//<params_n> ->, <type> VAR_ID <params_n>
//<params_n>->epsilon
int params_n(Token *token)
{
    if (token->t == R_PAR)
    { // next token is )
        return 2;
    }
    else if (token->t == COMMA)
    {
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

    while (token->t != SEMICOL && token->t != R_PAR)
    {
        addTokenToExpression(exp, token);
        dtorToken(token);
        token = getToken();
    }
    printf("TU\n");

    if (token->t == SEMICOL)
    {
        ungetc(';', stdin);
    }
    else
    {
        ungetc(')', stdin);
    }

    if (bottomUp(exp))
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

int condition(Token *token)
{
    if (token->t == IF)
    { // IF
        dtorToken(token);
        token = getToken();
        if (token->t == L_PAR)
        { // IF (
            dtorToken(token);
            token = getToken();
            if (expression(token) == 1)
            { // IF ( <expression>
                dtorToken(token);
                token = getToken();
                if (token->t == R_PAR)
                { // IF ( <expression> )
                    dtorToken(token);
                    token = getToken();
                    if (token->t == L_CPAR)
                    { // IF ( <expression> ) {
                        dtorToken(token);
                        token = getToken();
                        if (statement(token))
                        { // IF ( <expression> ) { <statement>
                            dtorToken(token);
                            token = getToken();
                            if (token->t == R_CPAR)
                            { // IF ( <expression> ) { <statement> }
                                dtorToken(token);
                                token = getToken();
                                if (token->t == ELSE)
                                { // IF ( <expression> ) { <statement> } ELSE
                                    dtorToken(token);
                                    token = getToken();
                                    if (token->t == L_CPAR)
                                    { // IF ( <expression> ) { <statement> } ELSE {
                                        dtorToken(token);
                                        token = getToken();
                                        if (statement(token))
                                        { // IF ( <expression> ) { <statement> } ELSE { <statement>
                                            dtorToken(token);
                                            token = getToken();
                                            if (token->t == R_CPAR)
                                            { // IF ( <expression> ) { <statement> } ELSE { <statement> }
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

int function_call(Token *token)
{
    if (token->t == ID)
    { // ID
        dtorToken(token);
        token = getToken();
        if (token->t == L_PAR)
        { // ID (
            dtorToken(token);
            token = getToken();
            if (params(token))
            { // ID ( <params>
                dtorToken(token);
                token = getToken();
                if (token->t == R_PAR)
                { // ID ( <params> )
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
        dtorToken(token);
        token = getToken();
        if (token->t == L_PAR)
        { // while (
            dtorToken(token);
            token = getToken();
            if (expression(token) == 1)
            { // while ( <expression>
                dtorToken(token);
                token = getToken();
                if (token->t == R_PAR)
                { // while ( <expression> )
                    dtorToken(token);
                    token = getToken();
                    if (token->t == L_CPAR)
                    { // while ( <expression> ) {
                        dtorToken(token);
                        token = getToken();
                        if (statement(token))
                        { // while ( <expression> ) { <statement>
                            dtorToken(token);
                            token = getToken();
                            if (token->t == R_CPAR)
                            { // while ( <expression> ) { <statement> }
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

int var_rule(Token *token)
{
    if (token->t == VAR_ID || token->t == STRING || token->t == INT || token->t == FLOAT || token->t == NULL_KEYWORD)
    { // <var_rule> (VAR_ID/STRING/INT/FLOAT/NULL)
        return 1;
    }
    else if (token->t == ID)
    { // <var_rule> (<function_call> (ID))
        if (function_call(token) == 1)
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

// return 1 - vsetko je ok
// return 2 - ok ale nebudem statement -> mozno ine pravidlo
// return 0 - vypis error
int statement(Token *token)
{
    if (token->t == VAR_ID)
    { // VAR_ID
        dtorToken(token);
        token = getToken();
        if (token->t == EQ)
        { // VAR_ID =
            dtorToken(token);
            token = getToken();
            if (token->t == VAR_ID)
            { // VAR_ID = VAR_ID
                dtorToken(token);
                token = getToken();
                if (token->t == SEMICOL)
                { // VAR_ID = VAR_ID;
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
                    return 0;
                }
            }
            else if (var_rule(token) == 1)
            { // VAR_ID = <var_rule>
                dtorToken(token);
                token = getToken();
                if (token->t == SEMICOL)
                { // VAR_ID = <var_rule>;
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
                    dtorToken(token);
                    token = getToken();
                    if (token->t == SEMICOL)
                    { // VAR_ID = <expression>;
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
        dtorToken(token);
        token = getToken();
        if (var_rule(token) == 1)
        { // RETURN <var_rule>
            dtorToken(token);
            token = getToken();
            if (token->t == SEMICOL)
            { // RETURN <var_rule>;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if (var_rule(token) == 2)
        {
            dtorToken(token);
            token = getToken();
            if (expression(token) == 1)
            { // RETURN <expression>
                dtorToken(token);
                token = getToken();
                if (token->t == SEMICOL)
                { // RETURN <expression>;
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
        if (function_call(token) == 1)
        { // <function_call>
            dtorToken(token);
            token = getToken();
            if (token->t == SEMICOL)
            { // <function_call> ;
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
        return 2;
    }
}

int main()
{
    Token *token = getToken();
    if (prog(token))
    {
        dtorToken(token);
        return 0; // exit code 0
    }
    else
    {
        dtorToken(token);
        exit(2);
    }
}
