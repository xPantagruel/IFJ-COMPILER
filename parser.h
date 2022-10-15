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

bool bottomUp(Expression *exp);

Expression *initExpression();

void dtorExpression(Expression *exp);

bool addTokenToExpression(Expression *exp, Token *token);

bool prog(Token *t);

// return 1 - vsetko je ok
// return 2 - ok ale nebudem statement -> mozno ine pravidlo
// return 0 - vypis error
int statement(Token *t);

// return 1 - vsetko je ok
// return 2 - ok ale nebudem var_rule -> mozno ine pravidlo
// return 0 - vypis error
int var_rule(Token *t);

int expression(Token *t);

int condition(Token *t);

int while_rule(Token *t);

int function_call(Token *t);
