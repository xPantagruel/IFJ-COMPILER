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

Expression *initExpression()
{
    Expression *exp = malloc(sizeof(Expression));
    if (exp == NULL) { //malloc failed
        exit(99);
    }
    exp->arrayLen = 0;
    exp->tokenArray = NULL;

    return exp;
}

void dtorExpression(Expression *exp) {
    for (int i = 0; i < exp->arrayLen; i++) {
        free(exp->tokenArray[i]);
    }
    free(exp);
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


// enum pos {
//     PROG,
//     PARAMS,
//     PARAMS_N
// }
// static char rule[] = "start"; //daj do .h -> bude enum nie string

//start rule
// void start(Token *token) {
//     if (token->t == "statement") {
//         // ok
//         // dalsi token musi byt ID
//         rule = "ID";
//     } else  if (FUNCTION){
//         //nevyhovuje
//     } else {
//         //err
//     }
// }


int main()
{

    Token *token = getToken();
    while (strcmp(token->val, "EOF"))
    {




        // if (strcmp(rule, "start")) {
        //     start(token);
        // }
        // printf("%s %d\n", token->val, token->t);
        dtorToken(token);
        token = getToken();
    }
    // printf("%s\n", token->val);
    // print EOF
}
