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

#include "parser.h"

void initExpression(Expression *exp)
{
    exp->arrayLen = 0;
    exp->tokenArray = NULL;
}

bool addTokenToExpression(Expression *exp, Token *token)
{
    exp->tokenArray = realloc(exp->tokenArray, sizeof((exp->arrayLen + 1) * sizeof(Token *)));
    if (exp->tokenArray == NULL)
    {
        return false;
    }
    exp->tokenArray[exp->arrayLen] = token;
    exp->arrayLen++;
    return true;
}

int main()
{
    Token *token = getToken();
    while (strcmp(token->val, "EOF"))
    {
        printf("%s %d\n", token->val, token->t);
        dtorToken(token);
        token = getToken();
    }
    printf("%s\n", token->val);
    // print EOF
}
