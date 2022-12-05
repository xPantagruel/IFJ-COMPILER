#ifndef BOTTOM_UP_H
#define BOTTOM_UP_H

#include "parser.h"
#include "scanner.h"
#include "stack.h"
#include "parser.h"
#include "symtable.h"
#include "code_generation.h"
#include <stdbool.h>
#include <string.h>

enum termType
{
    NOTDEF_TERM = -1,
    MULT_DIV,
    PLUS_MINUS,
    CONCAT,
    COMPARE,
    EQUALS,
    L_BRACKET,
    R_BRACKET,
    I_INT,
    I_FLOAT,
    I_STRING,
    I_NULL,
    TOP_BOTTOM,
    EXPRESSION,
};

typedef struct term
{
    enum termType type;
    char *value;
    Term *childTerms[3];
    int termLen;
    bool isStop;
    Token *originalToken;
} Term;

Term *initTerm(char *value, int type, Token *originalToken);

void changeTermValue(Term *t, char *value);

int loadTermsToExpression(Term *expression, Stack *stack);

int convertTypesInReduce(Term *t1, Term *operator, Term * t2);

int convertPlusMinusMul(Term *t1, Term *t2);

int convertDiv(Term *t1, Term *t2);

int convertDot(Term *t1, Term *t2);

int convertCompareEquals(Term *t1, Term *t2);

int convertBrackets(Term *t);

int convertTermToTokenType(int type, int originalType);

int convertTokenToTermType(int type);

bool reduce(Stack *stack);

bool ruleDecider(Term *terms[3], int termCount);

bool ruleIdentifier(Term *t);

bool rulePlusMinus(Term *t1, Term *operator, Term * t2);

bool ruleConcat(Term *t1, Term *operator, Term * t2);

bool ruleMultDiv(Term *t1, Term *operator, Term * t2);

bool ruleBracket(Term *leftBracket, Term *t1, Term *rightBracket);

bool ruleCompare(Term *t1, Term *operator, Term * t2);

bool ruleEquals(Term *t1, Term *operator, Term * t2);

bool bottomUp(Expression *exp, int *resultType);

#endif
