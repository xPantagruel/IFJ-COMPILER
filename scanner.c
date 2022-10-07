#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void getProlog() { //TODO mozno musi byt medzera medzi php a declare
    char prologValidString[] = "<?phpdeclare(strict_types=1);";
    char prologString[strlen(prologValidString) + 1];
    
    int pos = 0; //actual position in new string
    int c;
    while (strlen(prologString) != strlen(prologValidString)) {
        c = getchar();
        if (c == '\n') {
            row++;
        }

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
        } else if ((')' <= c && c <= '/') || (';' <= c && c <= '>') || c == '!' || c == EOF) {
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

int skipLineComment() {
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
    if (c == '\n') {
        row++;
    }

    if (c == EOF) { //comment like: //this is my commentEOF
        return 0;
    }
    return 1;
}

void skipBlockComment() {
    int prevC = 0;
    int actualC = getchar();
    while ((prevC != '*' && actualC != '/') && actualC != EOF) {
        prevC = actualC;
        actualC = getchar();
    }

    if (actualC == EOF) { //comment like: /* this is my commentEOF
        exit(1);
    }
}

Token *getToken() {
    if (!prologFound) {
        getProlog();
        prologFound = 1;
    }

    Token *token = tokenInit();

    int tokenFound = 0;
    enum state actualState = START;
    enum type t = NOT_DEFINED;
    int c;

    while (!tokenFound){
        c = getchar();
        if (c == '\n') {
            row++;
        }

        switch(actualState) {
            case START:
                switch (c) {
                    case '$':
                        actualState = VAR_ID_S;
                        t = VAR_ID;
                        break;
                    case '/':
                        actualState = SLASH_S;
                        break;
                    case EOF:
                        t = EOF_T;
                        char eof_s[] = "EOF";
                        for (int i = 0; (unsigned long)i < strlen(eof_s); i++) {
                            addCharToToken(eof_s[i], token);
                        }
                        addTypeToToken(t, token);
                        addRowToToken(row, token);
                        tokenFound = 1;
                        //TODO check char after EOF
                        break;
                    default:
                        break;
                    //end of switch by char in START state
                }
                break;

            case VAR_ID_S:
                switch(checkId(c)) {
                    case 1:
                        addCharToToken(c, token);
                        break;
                    case 0:
                        addTypeToToken(t, token);
                        addRowToToken(row, token);
                        tokenFound = 1;
                        break; 
                    //end of switch by char in VAR_ID_S state
                }
                break;
            
            case SLASH_S:
                switch(c) {
                    case '/':
                        if (!skipLineComment()) {
                            ungetc(EOF, stdin);
                        }
                        actualState = START;
                        break;
                    case '*':
                        skipBlockComment();
                        actualState = START;
                        break;
                    
                    default:
                        t = SLASH;
                        ungetc(c, stdin);
                        addCharToToken('/', token);
                        addTypeToToken(t, token);
                        addRowToToken(row, token);
                        tokenFound = 1;
                        break;
                    //end of switch by char in SLASH_S state
                }
                break;
            
            default:
                break;
            //end of switch by actualState
        }

    }

    return token;
}

int main() {
    Token *token = getToken();
    while (strcmp(token->val, "EOF")) {
        printf("%s %d\n", token->val, token->row);
        free(token);
        token = getToken();
    }
    printf("%s %d\n", token->val, token->row);

}

//pridat do KA -> ; + EOF + konecny prolog
//myslet aj na to ze napr za var nemusi byt medzera + EOF nezabudnut
//pozret todo + okomentovat
