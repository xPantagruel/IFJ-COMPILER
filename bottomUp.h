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

/// @brief Initialize variable of type Term
/// @param value of the term
/// @param type of the term
/// @param originalToken token from which term is created
/// @return pointer to new term
Term *initTerm(char *value, int type, Token *originalToken);

/// @brief Ovewrite term value with new value
/// @param t term to overwrite value of
/// @param value new value
void changeTermValue(Term *t, char *value);

/// @brief Reduce terms to new term
/// @param expression term to reduce to
/// @param stack stack of terms
/// @return number of terms reduced
int loadTermsToExpression(Term *expression, Stack *stack);

/// @brief Set type of new term
/// @param t1
/// @param operator,Term*t2
/// @return type of new term
int convertTypesInReduce(Term *t1, Term *operator, Term * t2);

/// @brief Rule for +,-,*
/// @param t1 first term
/// @param t2 second term
/// @return type of new term
int convertPlusMinusMul(Term *t1, Term *t2);

/// @brief Rule for /
/// @param t1 first term
/// @param t2 second term
/// @return type of new term
int convertDiv(Term *t1, Term *t2);

/// @brief Rule for .
/// @param t1 first term
/// @param t2 second term
/// @return type of new term
int convertDot(Term *t1, Term *t2);

/// @brief Rule for ===,!== and relation operators
/// @param t1 first term
/// @param t2 second term
/// @return type of new term
int convertCompareEquals(Term *t1, Term *t2);

/// @brief Rule for ===,!==
/// @param t1 term
/// @return type of new term
int convertBrackets(Term *t);

/// @brief convert type of term to type of token
/// @param type of term
/// @param originalType type of original token of term
/// @return token type
int convertTermToTokenType(int type, int originalType);

/// @brief convert type of token to type of term
/// @param type of token
/// @return type of term
int convertTokenToTermType(int type);

/// @brief reduces terms to expression term
/// @param stack to reduce terms on
/// @return true on success, false on error
bool reduce(Stack *stack);

/// @brief runs terms through rules for reducing
/// @param terms to reduce
/// @param termCount number of terms
/// @return true on success, false on error
bool ruleDecider(Term *terms[3], int termCount);

/// @brief identifier rule
bool ruleIdentifier(Term *t);

/// @brief + - rule
bool rulePlusMinus(Term *t1, Term *operator, Term * t2);

/// @brief . rule
bool ruleConcat(Term *t1, Term *operator, Term * t2);

/// @brief * / rule
bool ruleMultDiv(Term *t1, Term *operator, Term * t2);

/// @brief () rule
bool ruleBracket(Term *leftBracket, Term *t1, Term *rightBracket);

/// @brief relation operators rule
bool ruleCompare(Term *t1, Term *operator, Term * t2);

/// @brief ===, !== rule
bool ruleEquals(Term *t1, Term *operator, Term * t2);

/// @brief main function, created derivation tree of the expression
/// @param exp expression to analyze
/// @param resultType pointer to integer to save type of root of the tree
/// @return true on success, false on error
bool bottomUp(Expression *exp, int *resultType);

#endif
