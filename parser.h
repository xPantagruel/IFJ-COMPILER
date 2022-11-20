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

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <stdbool.h>

typedef struct Exp
{
    Token **tokenArray;
    int arrayLen;
} Expression;

/** Auxiliary variable to know if we are in <condtion> <while> <function_declaration> rule */
static int iAmInConditionWhileFunRule = 0;

/**
 * @brief Function for initializing expression struct.
 *
 * @return pointer to initialized expression
 */
Expression *initExpression();

/**
 * @brief Expression destructor.
 *
 * @param exp pointer to expression
 */
void dtorExpression(Expression *exp);

/**
 * @brief Adding token to expression->tokenArray.
 *
 * @param exp pointer to expression
 * @param token pointer to token which will be added to expression
 */
void addTokenToExpression(Expression *exp, Token *token);

/**
 * @brief Implementation of rules: <prog> -> <statement> <prog>
                                   <prog> -> <function_declaration> <prog>
                                   <prog> -> EOF
 *
 * @param token pointer to token
 *
 * @return true Return true if syntax analysis is ok.
 * @return false Return false if something is wrong.
 */
bool prog(Token *token);

/**
 * @brief Implementation of rules:  <statement> -> VAR_ID = <var_rule> ; <statement>
                                    <statement> -> VAR_ID = <expression> ; <statement>
                                    <statement> -> VAR_ID = VAR_ID ; <statement>
                                    <statement> -> RETURN <var_rule> ;
                                    <statement> -> RETURN ;
                                    <statement> -> RETURN <expression> ;
                                    <statement> -> <condition> <statement>
                                    <statement> -> <while> <statement>
                                    <statement> -> <function_call> ; <statement>

                                    <statement> -> <function_declaration> <statement> -> if I am not in condition, while or in
                                                                                         function_declaration

                                    <statement> -> epsilon
 *
 * @param token  pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int statement(Token *token);

/**
 * @brief Implementation of rules:  <var_rule> -> <function_call>
                                    <var_rule> -> VAR_ID
                                    <var_rule> -> STRING
                                    <var_rule> -> number (int alebo float)
                                    <var_rule> -> NULL
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int var_rule(Token *token);

/**
 * @brief Implementation of rule <function_declaration> -> FUNCTION <function_call> : <type> { <statement> }.
 *
 * @param token pointer to token
 *
 * @return true Return true if syntax analysis is ok.
 * @return false Return false if something is wrong.
 */
bool function_declaration(Token *token);

/**
 * @brief Function which collect tokens to expression struct and calls bottomUp.
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               0 error in syntax
 */
int expression(Token *token);

/**
 * @brief Implementation of rule <condition> -> IF ( <expression> ) { <statement> } ELSE { <statement> }.
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int condition(Token *token);

/**
 * @brief Implementation of rule <while> -> while ( <expression> ) { <statement> }.
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int while_rule(Token *token);

/**
 * @brief Implementation of rule <function_call> -> ID ( <params> ).
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int function_call(Token *token, bool isDeclaration);

/**
 * @brief Implementation of rules:  <type> -> FLOAT_TYPE
                                    <type> -> INT_TYPE
                                    <type> -> STRING_TYPE
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int type(Token *token);

/**
 * @brief Implementation of rules:  <params> -> epsilon
                                    <params> -> <type> VAR_ID <params_n>
                                    <params> -> <type> STRING <params_n>
                                    <params> -> STRING <params_n>
                                    <params> -> VAR_ID <params_n>
                                    <params> -> <type> FLOAT/INT <params_n>
                                    <params> -> FLOAT/INT <params_n>
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int params(Token *token);

/**
 * @brief Implementation of rules:  <params_n> -> , <params>
                                    <params_n> -> epsilon
 *
 * @param token pointer to token
 *
 * @return int 1 syntax is ok
               2 no rule was matched -> we maybe can use other rule
               0 error in syntax
 */
int params_n(Token *token);

/**
 * @brief Function which push token value back to stdin.
 *
 * @param token pointer to token
 */
void pushTokenToStdin(Token *token);

void freeAndExit(int code, char *message);

#endif

/*** End of parser.h ***/
