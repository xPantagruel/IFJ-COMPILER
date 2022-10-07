#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void getProlog() {
    char prologValidString[] = "<?phpdeclare(strict_types=1);";
    char prologString[strlen(prologValidString) + 1];
    
    int pos = 0; //actual position in new string
    int c;
    while (strlen(prologString) != strlen(prologValidString)) {
        c = getchar();
        if (!isspace(c)) { //ignoring spaces
            prologString[pos] = c;
            ++pos;
            prologString[pos] = '\0';
        }
    }

    if (strcmp(prologString, prologValidString)) {
        exit(2);
    }
}

void addCharToToken(int c, Token *token) {
    char tmp[] = {c, '\0'};

    if (token->val == NULL) {
        token->val = malloc(sizeof(char)*2);
    } else {
        token->val = realloc(token->val, sizeof(char)*(strlen(token->val) + 2));
    }

    if (token->val == NULL) {
        exit(99);
    } 

    token->val = strncat(token->val, tmp, 1);
}

Token *tokenInit() {
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
        exit(99);
    }

    token->row = -1;
    token->val = NULL;
    token->t = NOT_DEFINED;

    return token;
}

int checkId(int c) {
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('_' == c) || ('0' <= c && c <= '9')) {
        return 1;
    } else {
        if (isspace(c)) {
            return 0;
        } else if ((')' <= c && c <= '/') || (';' <= c && c <= '>') || c == '!') {
            ungetc(c, stdin);
            return 0;
        } else {
            exit(1);
        }
    }
}

void addRowToToken(int row, Token *token) {
    token->row = row;
}

void addTypeToToken(enum type t, Token *token) {
    token->t = t;
}

Token *getToken() {
    if (!prologFound) {
        getProlog();
        prologFound = 1;
    }

    Token *token = tokenInit();

    int tokenFound = 0;
    enum type actualState = START;
    int c;
    int row = 0;
    while (!tokenFound){
        c = getchar();

        if (c == '\n') {
            row++;
        }

        switch(actualState) {
            case START:
                switch (c) {
                    case '$':
                        actualState = VAR_ID;
                        break;

                    default:
                        break;
                }
                break;

            case VAR_ID:
                switch(checkId(c)) {
                    case 1:
                        addCharToToken(c, token);
                        break;
                    case 0:
                        addTypeToToken(actualState, token);
                        addRowToToken(row, token);
                        tokenFound = 1;
                        break;
                }
                break;
            
            default:
                break;
        }

    }

    return token;
}

//remove me
//todo case atd + konecny prolog + ; + EOF
int main() {
    Token *token = getToken();
    printf("%s\n", token->val);
    free(token);
}
