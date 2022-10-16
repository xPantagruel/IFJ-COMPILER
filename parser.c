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

bool prog(Token * token) {
    (void)token;
    return true; 
}

int type(Token * token) {
    (void)token;
    return 1;
}

int params(Token * token) {
    (void)token;
    return 1;
}

int params_n(Token * token) {
    (void)token;
    return 1;
}

int expression(Token * token) {
    (void)token;
    return 1;
}

int condition(Token * token) {
    (void)token;
    return 1;
}

int function_call(Token * token) {
    (void)token;
    return 1;
}

int while_rule(Token *token) {
    if (token->t == WHILE) { // while
        token = getToken();
        if (token->t == L_PAR) { //while (
            token = getToken();
            if (expression(token) == 1) { // while ( <expression>
                token = getToken();
                if (token->t == R_PAR) { // while ( <expression> )
                    token = getToken();
                    if (token->t == L_CPAR) { // while ( <expression> ) {
                        token = getToken();
                        if (statement(token) == 1 || statement(token) == 2) { // while ( <expression> ) { <statement>
                            token = getToken();
                            if (token->t == R_CPAR) { // while ( <expression> ) { <statement> }
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
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 2;
    }
}

int var_rule(Token *token) {
    if (token->t == VAR_ID || token->t == STRING || token->t == INT || token->t == FLOAT || token->t == NULL_KEYWORD) { // <var_rule> (VAR_ID/STRING/INT/FLOAT/NULL) 
        return 1;
    } else if (token->t == ID) { // <var_rule> (<function_call> (ID))
        if (function_call(token) == 1) { // <var_rule> (<function_call>)
            return 1;
        } else {
            return 0;
        }
    } else {
        return 2; 
    }
}


// return 1 - vsetko je ok
// return 2 - ok ale nebudem statement -> mozno ine pravidlo
// return 0 - vypis error
int statement(Token *token) { 
    if (token->t == VAR_ID) { //VAR_ID
        token = getToken();
        if (token->t == EQ) { //VAR_ID = 
            token = getToken();
            if (token->t == VAR_ID) { // VAR_ID = VAR_ID
                token = getToken();
                if (token->t == SEMICOL) { // VAR_ID = VAR_ID;
                    if (statement(getToken()) == 1) { // VAR_ID = VAR_ID; <statement>
                        return 1;
                    } else {
                        return 0; 
                    }
                } else {
                    return 0;
                }
            } else if (var_rule(token) == 1) { // VAR_ID = <var_rule>
                token = getToken();
                if (token->t == SEMICOL) { // VAR_ID = <var_rule>;
                    if (statement(getToken()) == 1) { // VAR_ID = <var_rule>; <statement>
                        return 1;
                    } else {
                        return 0; 
                    }
                } else {
                    return 0;
                }
            } else if (var_rule(token) == 2) {
                if (expression(token) == 1) { // VAR_ID = <expression>
                    token = getToken();
                    if (token->t == SEMICOL) { // VAR_ID = <expression>;
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
    } else if (token->t == RETURN) { // RETURN
        token = getToken();
        if (var_rule(token) == 1) { // RETURN <var_rule>
            token = getToken();
            if (token->t == SEMICOL) { // RETURN <var_rule>;
                return 1;
            } else {
                return 0;
            }
        } else if (var_rule(token) == 2) {
            token = getToken();
            if (expression(token) == 1) { // RETURN <expression>
                token = getToken();
                if (token->t == SEMICOL) { // RETURN <expression>;
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
    } else if (token->t == IF){ // <condtion> (IF)
        if (condition(token) == 1) { // <condition>
            token = getToken();
            if (statement(token) == 1) { // <condition> <statement>
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }

    } else if (token->t == WHILE) { //<while> (WHILE)
        if (while_rule(token) == 1) { // <while>
            token = getToken();
            if (statement(token) == 1) { // <while> <statement>
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (token->t == ID){ // <function_call> (ID)
        if (function_call(token) == 1) { // <function_call>
            token = getToken();
            if (token->t == SEMICOL) { // <function_call> ;
                token = getToken();
                if (statement(token) == 1) { // <function_call> ; <statement>
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
