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

bool prog(Token *t) {
    (void)t;
    return true;
}
int var_rule(Token *t) {
    (void)t;
    return 1;
    // if (t->t == STRING || t->t == STRING || t->t == INT || t->t == FLOAT || t->t == NULL) { // VAR_ID = <var_rule> (VAR_ID/STRING/INT/FLOAT/NULL) 
    //     t = getToken();
    //     if (t->t == SEMICOL) { // VAR_ID = <var_rule>;
    //         r = STATEMENT; // VAR_ID = <var_rule>; <statement>
    //         return true;
    //     } else {
    //         return false; //TODO Exit
    //     }
    // } else if (t->t == ID) { // VAR_ID = <var_rule> (<function_call>)
    //     t = getToken();
    //     if (t->t == L_PAR) { // VAR_ID = ID (
    //         //chcem param  a potom ) 
    //     } else {
    //         return false; //TODO EXIT
    //     }
    // } else {
    //     return false; //TODO EXIT
    // }
}

int expression(Token *t) {
    (void)t;
    return 1;
}

int condition(Token *t) {
    (void)t;
    return 1;
}

int while_rule(Token *t) {
    (void)t;
    return 1;
}

int function_call(Token *t) {
    (void)t;
    return 1;
}



// MATEJ READ ME
// return 1 - vsetko je ok
// return 2 - ok ale nebudem statement -> mozno ine pravidlo
// return 0 - vypis error
int statement(Token *t) { 
    if (t->t == VAR_ID) { //VAR_ID
        t = getToken();
        if (t->t == EQ) { //VAR_ID = 
            t = getToken();
            if (t->t == VAR_ID) { // VAR_ID = VAR_ID
                t = getToken();
                if (t->t == SEMICOL) { // VAR_ID = VAR_ID;
                    if (statement(getToken()) == 1) { // VAR_ID = VAR_ID; <statement>
                        return 1;
                    } else {
                        return 0; 
                    }
                } else {
                    return 0;
                }
            } else if (var_rule(t) == 1) { // VAR_ID = <var_rule>
                t = getToken();
                if (t->t == SEMICOL) { // VAR_ID = <var_rule>;
                    if (statement(getToken()) == 1) { // VAR_ID = <var_rule>; <statement>
                        return 1;
                    } else {
                        return 0; 
                    }
                } else {
                    return 0;
                }
            } else if (var_rule(t) == 2) {
                if (expression(t) == 1) { // VAR_ID = <expression>
                    t = getToken();
                    if (t->t == SEMICOL) { // VAR_ID = <expression>;
                        if (statement(getToken()) == 1) { // VAR_ID = <expression>; <statement>
                            return 1;
                        } else {
                            return 0; 
                        }
                    } else {
                        return 0;
                    }
                } else { // no more options -> error
                    return 0; 
                }
            } else { 
                return 0;
            }
        } else {
            return 0;
        }
    } else if (t->t == RETURN) { // RETURN
        t = getToken();
        if (var_rule(t) == 1) { // RETURN <var_rule>
            t = getToken();
            if (t->t == SEMICOL) { // RETURN <var_rule>;
                return 1;
            } else {
                return 0;
            }
        } else if (var_rule(t) == 2) {
            t = getToken();
            if (expression(t) == 1) { // RETURN <expression>
                t = getToken();
                if (t->t == SEMICOL) { // RETURN <expression>;
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (t->t == IF){ // <condtion> (IF)
        if (condition(t) == 1) { // <condition>
            t = getToken();
            if (statement(t) == 1) { // <condition> <statement>
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }

    } else if (t->t == WHILE) { //<while> (WHILE)
        if (while_rule(t) == 1) { // <while>
            t = getToken();
            if (statement(t) == 1) { // <while> <statement>
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (t->t == ID){ // <function_call> (ID)
        if (function_call(t) == 1) { // <function_call>
            t = getToken();
            if (t->t == SEMICOL) { // <function_call> ;
                t = getToken();
                if (statement(t) == 1) { // <function_call> ; <statement>
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else { // epsilon
        return 2;
    }
}

int main()
{   
    // TODO ako rozoznam epsilon -> ze napr nebude statement ale rovno }
    // TODO FREE TOKENS when calling getToken again
    if (prog(getToken())) { //STACI TO IBA TAKTO BEZ ENUM RULE A BEZ GLOBALNEJ PREMENNEJ
        return 1;
    } else {
        exit(2);
    }
}
