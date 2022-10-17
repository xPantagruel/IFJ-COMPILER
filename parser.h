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
//#include "bottomUp.h"
#include <stdbool.h>

typedef struct Exp
{
    Token **tokenArray;
    int arrayLen;
} Expression;

static int iAmInConditionWhileFunRule = 0;

bool bottomUp(Expression *exp);

Expression *initExpression();

void dtorExpression(Expression *exp);

void addTokenToExpression(Expression *exp, Token *token);

bool prog(Token *token);

// return 1 - vsetko je ok
// return 2 - ok ale nebudem statement -> mozno ine pravidlo
// return 0 - vypis error
int statement(Token *token);

// return 1 - vsetko je ok
// return 2 - ok ale nebudem var_rule -> mozno ine pravidlo
// return 0 - vypis error
int var_rule(Token *token);

bool function_declaration(Token *token);

int expression(Token *token);

int condition(Token *token);

int while_rule(Token *token);

int function_call(Token *token);

int type(Token *token);

int params(Token *token);

int params_n(Token *token);
