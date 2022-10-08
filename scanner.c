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
            pos++;
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
        } else if (('(' <= c && c <= '/') || (';' <= c && c <= '>') || c == '!' || c == EOF) { //TODO nema byt aj {} atd..
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

void setOneCharToken(Token *token, int c, int row, enum type t) {
    addCharToToken(c, token);
    addTypeToToken(t, token);
    addRowToToken(row, token);
}

int isOkAfterNum(int c) {
    //TODO toto kuknut este ci tam je vsetko
    if (c == EOF || isspace(c) || c == '!' || ('(' <= c && c <= '-') || c == '/' || ('<' <= c && c <= '>') || c == ';') {
        return 1;
    } else {
        return 0;
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

    int hexEscCount = 0;
    int oktEscCount = 0;

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
                            case ':':
                                setOneCharToken(token, ':', row, COLON);
                                tokenFound = 1;
                                break;
                            case '(':
                                setOneCharToken(token, '(', row, L_PAR);
                                tokenFound = 1;
                                break;
                            case ')':
                                setOneCharToken(token, ')', row, R_PAR);
                                tokenFound = 1;
                                break;
                            case '{':
                                setOneCharToken(token, '{', row, L_CPAR);
                                tokenFound = 1;
                                break;
                            case '}':
                                setOneCharToken(token, '}', row, R_CPAR);
                                tokenFound = 1;
                                break;
                            case ';':
                                setOneCharToken(token, ';', row, SEMICOL);
                                tokenFound = 1;
                                break;
                            case ',':
                                setOneCharToken(token, ',', row, COMMA);
                                tokenFound = 1;
                                break;
                            case '+':
                                setOneCharToken(token, '+', row, PLUS);
                                tokenFound = 1;
                                break;
                            case '-':
                                setOneCharToken(token, '-', row, MINUS);
                                tokenFound = 1;
                                break;
                            case '.':
                                setOneCharToken(token, '.', row, DOT);
                                tokenFound = 1;
                                break;
                            case '*':
                                setOneCharToken(token, '*', row, MUL);
                                tokenFound = 1;
                                break;
                            case '?':
                                actualState = ID_S;
                                break;
                            case '=':
                                actualState = EQ_S;
                                addCharToToken('=', token);
                                break;
                            case '<':
                                actualState = LESS_MORE_S;
                                addCharToToken('<', token);
                                break;
                            case '>':
                                actualState = LESS_MORE_S;
                                addCharToToken('>', token);
                                break;
                            case '!':
                                actualState = NOT_EQ_S;
                                addCharToToken('!', token);
                                break;
                            case '"':
                                actualState = STRING_S;
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
                                if (checkId(c)) {
                                    if (isdigit(c)) {
                                        actualState = NUM_S;
                                        t = INT;
                                    } else {
                                        actualState = ID_S;
                                    }
                                    ungetc(c, stdin);
                                }
                                break;
                            //end of switch by char in START state
                    }
                break;
            case STRING_S:
                switch (isalpha(c)) {
                    case 1:
                        addCharToToken(c, token);
                        break;
                    case 0:
                        switch(isnumber(c)) {
                            case 1:
                                //ak je backslash predtym + ak je backslash a x predtym + ak nie je nic
                                if (token->val[strlen(token->val)-1] == 'x' && token->val[strlen(token->val)-2] == '\\') {
                                    hexEscCount = 0;
                                    actualState = GET_HEX_S;
                                    ungetc(c, stdin);
                                } else if (token->val[strlen(token->val)-1] == '\\') {
                                    oktEscCount = 0;
                                    actualState = GET_OKT_S;
                                    ungetc(c, stdin);
                                } else {
                                    addCharToToken(c, token);
                                }
                                break;
                            case 0:
                                switch (c) {
                                    case '"':
                                        if (token->val[strlen(token->val)-1] == '\\') {
                                            addCharToToken(c, token);
                                        } else {
                                            addTypeToToken(STRING, token);
                                            addRowToToken(row, token);
                                            actualState = START;
                                            tokenFound = 1;
                                        }
                                        break;
                                    case '$':
                                        if (token->val[strlen(token->val)-1] == '\\') {
                                            addCharToToken(c, token);
                                        } else {
                                            exit(1);
                                        }
                                        break;
                                    default:
                                        addCharToToken(c, token);
                                        break;
                                }
                                break;
                        }
                        break;
                }
                break;

            case GET_HEX_S:
                switch (hexEscCount) {
                    case 0:
                        if (('0' <= c && c <= '9') || ('a' <= tolower(c) && tolower(c) <= 'f')) {
                            addCharToToken(tolower(c), token);
                        } else {
                            exit(1);
                        }
                        hexEscCount++;
                        break;
                    case 1:
                        if (token->val[strlen(token->val)-1] == '0') {
                            if (('1' <= c && c <= '9') || ('a' <= tolower(c) && tolower(c) <= 'f')) {
                                addCharToToken(tolower(c), token);
                            } else {
                                exit(1);
                            }
                        } else {
                            if (('0' <= c && c <= '9') || ('a' <= tolower(c) && tolower(c) <= 'f')) {
                                addCharToToken(tolower(c), token);
                            } else {
                                exit(1);
                            }
                        }
                        actualState = STRING_S;
                        break;      
                }
                break;

            case GET_OKT_S: 
                switch(oktEscCount) {
                    case 0:
                            if ('0' <= c && c <= '3') {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                        oktEscCount++;
                        break;
                    case 1:
                        if (token->val[strlen(token->val)-1] == '3') {
                            if ('1' <= c && c <= '7') {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                        } else {
                            if ('0' <= c && c <= '9') {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                        }
                        oktEscCount++;
                        break;
                    case 2:
                        if ((token->val[strlen(token->val)-2] == '3') && (token->val[strlen(token->val)-1] == '7')) {
                            if ('0' <= c && c <= '7') {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                        } else {
                            if ('0' <= c && c <= '9') {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                        }
                        actualState = STRING_S;
                        break;
                }
                break;
            case NOT_EQ_S:
                switch(c) {
                    case '=':
                        if (strlen(token->val) < 3) {
                            addCharToToken(c, token);
                        } else {
                            exit(1);
                        }
                        break;
                    default:
                        if (strlen(token->val) != 3) {
                            exit(1);
                        } else {
                            addTypeToToken(NOT_EQ, token);
                            addRowToToken(row, token);
                            actualState = START;
                            tokenFound = 1;
                        }
                        break;
                }

                break;
            case LESS_MORE_S:
                switch(c) {
                        case '=':
                            if (strlen(token->val) == 1) {
                                addCharToToken(c, token);
                            } else {
                                exit(1);
                            }
                            break;
                        default:
                            ungetc(c, stdin);
                            switch(token->val[0]) {
                                    case '<':
                                        switch(token->val[1]) {
                                            case '=': // <=
                                                t = LESS_EQ;
                                                break;
                                            default: // <
                                                t = LESS;
                                                break;
                                        }
                                        break;
                                    case '>':
                                        switch(token->val[1]) {
                                            case '=': // >=
                                                t = MORE_EQ;
                                                break;
                                            default: // >
                                                t = MORE;
                                                break;
                                        }
                                    break;
                            }
                            
                            addTypeToToken(t, token);
                            addRowToToken(row, token);
                            actualState = START;
                            tokenFound = 1;
                            break;

                }
                break;
            case EQ_S:
                switch(c) {
                        case '=':
                            if (strlen(token->val) < 3) {
                                addCharToToken(c, token);
                            } else {
                                exit(1); // ==== and more
                            }
                            break;
                        default:
                            switch (strlen(token->val)) {
                                case 1: // =
                                    ungetc(c, stdin);
                                    addRowToToken(row, token);
                                    addTypeToToken(EQ, token);
                                    actualState = START;
                                    tokenFound = 1;
                                    break;
                                case 3: // ===
                                    ungetc(c, stdin);
                                    addRowToToken(row, token);
                                    addTypeToToken(THREE_EQ, token);
                                    actualState = START;
                                    tokenFound = 1;
                                    break;
                                default: // ==
                                    exit(1);
                            }
                            break;
                }
                break;
            
            case NUM_S:
                switch (isdigit(c)) { //first will be for sure digit (default case above)
                        case 1:
                            addCharToToken(c, token);
                            break;
                        case 0:
                            switch(tolower(c)) {
                                case  '.':
                                    if (strchr(token->val, '.') != NULL) {
                                        exit(1);
                                    }
                                    t = FLOAT;
                                    actualState = NUM_NEEDED_S;
                                    addCharToToken(c, token);
                                    break;
                                case 'e':
                                    if (token->val[strlen(token->val)-1] == '.' || strchr(token->val, 'e') != NULL) {
                                        exit(1);
                                    }
                                    actualState = NUM_OR_PLUSMIN_NEEDED_S;
                                    addCharToToken(c, token);
                                    break;
                                case '+':
                                    if (token->val[strlen(token->val)-1] != 'e') {
                                        exit(1);
                                    }
                                    actualState = NUM_NEEDED_S;
                                    addCharToToken(c, token);
                                    break;
                                case '-':
                                    if (token->val[strlen(token->val)-1] != 'e') {
                                        exit(1);
                                    }
                                    actualState = NUM_NEEDED_S;
                                    addCharToToken(c, token);
                                    break;
                                default:
                                    if (isOkAfterNum(c)) {
                                        actualState = START;
                                        addRowToToken(row, token);
                                        addTypeToToken(t, token);
                                        tokenFound = 1;
                                        ungetc(c, stdin);
                                    } else {
                                        exit(2);
                                    }
                                    break;
                            }
                            break;
                }
                break;

            case NUM_NEEDED_S:
                if (isnumber(c)) {
                    addCharToToken(c, token);
                    actualState = NUM_S;
                } else {
                    exit(1);
                }

                break;

            case NUM_OR_PLUSMIN_NEEDED_S:
                if (isnumber(c) || c == '+' || c == '-') {
                    addCharToToken(c, token);
                    actualState = NUM_S;
                } else {
                    exit(1);
                }
                break;
                
            case ID_S:
                    switch(checkId(c)) {
                            case 1:
                                addCharToToken(c, token);
                                break;
                            case 0: {
                                if (!strcmp(token->val, "else")) {
                                    t = ELSE;
                                } else if (!strcmp(token->val, "float")) {
                                    t = FLOAT_TYPE;
                                } else if (!strcmp(token->val, "function")) {
                                    t = FUNCTION;
                                } else if (!strcmp(token->val, "if")) {
                                    t = IF;
                                } else if (!strcmp(token->val, "int")) {
                                    t = INT_TYPE;
                                } else if (!strcmp(token->val, "null")) {
                                    t = NULL_KEYWORD;
                                } else if (!strcmp(token->val, "return")) {
                                    t = RETURN;
                                } else if (!strcmp(token->val, "string")) {
                                    t = STRING_TYPE;
                                } else if (!strcmp(token->val, "void")) {
                                    t = VOID;
                                } else if (!strcmp(token->val, "while")) {
                                    t = WHILE;
                                } else {
                                    t = ID;
                                }

                                //end of switch by token value
                                
                                addTypeToToken(t, token);
                                addRowToToken(row, token);
                                tokenFound = 1;
                                break;
                            } //end of case 0

                            //end of switch by char in ID_S state
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

int main() { //TODO REMOVE ME
    Token *token = getToken();
    while (strcmp(token->val, "EOF")) {
        printf("%s %d %d\n", token->val, token->row, token->t);
        free(token);
        token = getToken();
    }
    printf("%s %d %d\n", token->val, token->row, token->t);

}

//TODO
//konecny prolog
//pozret todo
//okomentovat
//not found osetrit
//prejst exity - ci vobec tam maju byt a nie az v syntax
//zle rata row lebo ak dam ungetc \n tak to da row++
//printf vymazat
