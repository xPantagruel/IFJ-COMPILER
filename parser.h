/**
 * @file parser.h
 *
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 *
 * @brief Declatarions of enums, functions and variables used in parser.c.
 *
 * @date 2022-10-12
 */

#include "scanner.h"
#include <stdbool.h>

typedef struct Expression
{
    Token **tokenArray;
    int arrayLen;
} Expression;

bool topDown(Expression *exp);

Expression *initExpression();

void dtorExpression(Expression *exp);

bool addTokenToExpression(Expression *exp, Token *token);
