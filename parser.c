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

static char rule[] = "start"; //daj do .h

//start rule
void start(Token *token) {
    if (token->t == FUNCTION) {
        // ok
        // dalsi token musi byt ID
        rule = "ID";
    } else {
        //nevyhovuje
    }
}

void chcemId() {
    //if token == ID
        //ok -> chcem (
}

int main()
{

    Token *token = getToken();
    while (strcmp(token->val, "EOF"))
    {
        if (strcmp(rule, "start")) {
            start(token);
        }


        // printf("%s %d\n", token->val, token->t);
        dtorToken(token);
        token = getToken();
    }
    // printf("%s\n", token->val);
    // print EOF
}
