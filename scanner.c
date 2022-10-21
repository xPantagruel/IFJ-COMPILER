/**
 * @file scanner.c
 *
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 *
 * @brief Implementation of FSM automata for lexical analysis of the subset of php language.
 *
 * @date 2022-10-08
 */

#include "scanner.h"

void getProlog()
{
    char prologValidString[] = "<?php declare(strict_types=1);"; // prolog string
    char prologString[strlen(prologValidString) + 1];            // array of chars where chars from getchar() will be saved

    int pos = 0;          // actual position in new string
    int c;                // char
    int spaceCounter = 0; // auxiliary counter which allow us to count if we passed space char
    while (strlen(prologString) != strlen(prologValidString))
    {
        c = getchar();

        if (c == '/')
        { // skip comment in prolog
            c = getchar();
            if (c == '/')
            {
                skipLineComment();
                continue;
            }
            else if (c == '*')
            {
                skipBlockComment(tokenInit());
                continue;
            }
            else
            {
                exit(2);
            }
        }

        if (c == '\n')
        { // counting on which line from stdin we are
            row++;
        }

        if (isspace(c) && spaceCounter == 0)
        { // adding space to prolog
            prologString[pos] = ' ';
            pos++;
            prologString[pos] = '\0';
            spaceCounter++;
        }

        if (!isspace(c))
        { // ignoring spaces
            prologString[pos] = c;
            pos++;
            prologString[pos] = '\0';
        }
    }

    if (strcmp(prologString, prologValidString))
    {
        exit(2);
    }
}

void dtorToken(Token *token)
{

    if (token && token->val != NULL)
    {
        free(token->val);
    }

    if (token)
    {
        free(token);
    }
}

void addCharToToken(int c, Token *token)
{
    char tmp[] = {c, '\0'}; // creating "string" so we can use strncat

    if (token && token->val == NULL)
    {
        token->val = calloc(2, sizeof(char));
        if (!token->val)
        {
            exit(99);
        }
        strcpy(token->val, tmp);
    }
    else
    {
        token->val = realloc(token->val, (strlen(token->val) + 1) * sizeof(char));
        if (!token->val)
        {
            exit(99);
        }
    }
}

Token *tokenInit()
{
    Token *token = malloc(sizeof(Token));
    if (token == NULL)
    { // malloc failed
        exit(99);
    }

    // adding init values to token
    token->row = -1;
    token->val = NULL;
    token->t = NOT_DEFINED;

    return token;
}

void unGetC(int c)
{
    if (c != '\n')
    { // explained in scanner.h
        ungetc(c, stdin);
    }
}

int checkId(int c, Token *token)
{
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('_' == c) || ('0' <= c && c <= '9'))
    { // chars from which ID consists
        return 1;
    }
    else
    {
        if (isspace(c))
        {
            return 0;
        }
        else if (('(' <= c && c <= '/') || (';' <= c && c <= '>') || c == '!' || c == EOF || c == '{' || c == '}')
        { // chars that can be after ID
            unGetC(c);
            return 0;
        }
        else
        { // invalid char after ID
            error(2, token);
            return -1;
        }
    }
}

void addRowToToken(int row, Token *token)
{
    token->row = row;
}

void addTypeToToken(enum type t, Token *token)
{
    token->t = t;
}

int skipLineComment()
{
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
    if (c == '\n')
    {
        row++;
    }

    if (c == EOF)
    { // comment like: //this is my commentEOF
        return 0;
    }
    return 1;
}

void skipBlockComment(Token *token)
{
    int prevC = 0; // storing previous char
    int actualC = getchar();
    while ((prevC != '*' && actualC != '/') && actualC != EOF)
    {
        prevC = actualC;
        actualC = getchar();
    }

    if (actualC == EOF)
    { // comment like: /* this is my commentEOF
        error(1, token);
    }
}

void setOneCharToken(Token *token, int c, int row, enum type t)
{
    addCharToToken(c, token);
    addTypeToToken(t, token);
    addRowToToken(row, token);
}

int isOkAfterNum(int c)
{
    if (c == EOF || isspace(c) || c == '!' || ('(' <= c && c <= '-') || c == '/' || (';' <= c && c <= '>'))
    { // chars that are valid after number
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkClosingProlog()
{
    int c = getchar();
    if (c == '>')
    {
        if (getchar() == EOF)
        {
            return 1;
        }
        else
        {
            exit(2);
        }
    }
    unGetC(c);
    return 0;
}

void addQuestionMark(Token *token)
{
    memmove(token->val + 1, token->val, strlen(token->val) + 1);
    *token->val = '?';
}

void error(int errID, Token *token)
{
    dtorToken(token);
    exit(errID);
}

Token *getToken()
{
    if (!prologFound)
    { // we need to skip and validate prolog
        getProlog();
        prologFound = 1; // prolog found
    }

    Token *token = tokenInit();

    int tokenFound = 0; // auxiliary variable which we use to know if token was found -> we can return token
    enum state actualState = START;
    enum type t = NOT_DEFINED;
    int c; // char -> getchar()

    while (!tokenFound)
    {
        c = getchar();

        if (c == '\n')
        { // counting rows (stdin)
            row++;
        }

        switch (actualState)
        {
        case START:
            switch (c)
            {
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
                if (checkClosingProlog())
                {
                    // creating EOF as last token
                    t = EOF_T;
                    char eof_s[] = "EOF";

                    for (int i = 0; (unsigned long)i < strlen(eof_s); i++)
                    {
                        addCharToToken(eof_s[i], token);
                    }

                    addTypeToToken(t, token);
                    addRowToToken(row, token);
                    tokenFound = 1;

                    break;
                }

                actualState = ID_S;
                questionMark = 1;
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
                for (int i = 0; (unsigned long)i < strlen(eof_s); i++)
                {
                    addCharToToken(eof_s[i], token);
                }
                addTypeToToken(t, token);
                addRowToToken(row, token);
                tokenFound = 1;
                break;
            default:
                if (checkId(c, token))
                { // updating state to ID or NUM
                    if (isdigit(c))
                    {
                        actualState = NUM_S;
                        t = INT;
                    }
                    else
                    {
                        actualState = ID_S;
                    }
                    unGetC(c);
                }
                break;
                // end of switch by char in START state
            }
            break;
            // end of case START

        case STRING_S:
            switch (isalpha(c))
            {
            case 1: // alpha is ok
                addCharToToken(c, token);
                break;
            case 0: // not alpha
                switch (isdigit(c))
                {
                case 1: // is number
                    addCharToToken(c, token);
                    break;
                case 0: // not number
                    switch (c)
                    {
                    case '"':
                        if (token->val[strlen(token->val) - 1] == '\\')
                        { // char " in string
                            addCharToToken(c, token);
                        }
                        else
                        { // end of string
                            addTypeToToken(STRING, token);
                            addRowToToken(row, token);
                            actualState = START;
                            tokenFound = 1;
                        }
                        break;
                    case '$':
                        if (token->val[strlen(token->val) - 1] == '\\')
                        { // $ in string must be like "\$"
                            addCharToToken(c, token);
                        }
                        else
                        {
                            error(1, token);
                        }
                        break;
                    default: // other chars
                        addCharToToken(c, token);
                        break;
                    }
                    break;
                    // end of case 0
                }
                // end of switch by isdigit()
                break;
                // end of case 0
            }
            // end of switch by isalpha()
            break;
            // end of case STRING_S

        case NOT_EQ_S:
            switch (c)
            {
            case '=':
                if (strlen(token->val) < 3)
                { // != or !
                    addCharToToken(c, token);
                }
                else
                { // !=== -> want to add more =
                    error(1, token);
                }
                break;
            default: // not =
                if (strlen(token->val) != 3)
                { // !==
                    error(1, token);
                }
                else
                {
                    addTypeToToken(NOT_EQ, token);
                    addRowToToken(row, token);
                    actualState = START;
                    tokenFound = 1;
                }
                break;
            }

            break;
            // end of case NOT_EQ_S case

        case LESS_MORE_S:
            switch (c)
            {
            case '=':
                if (strlen(token->val) == 1)
                { // < or >
                    addCharToToken(c, token);
                }
                else
                { // <= or >= -> want to add more =
                    error(1, token);
                }
                break;
            default:
                unGetC(c);
                switch (token->val[0])
                {
                case '<': // less
                    switch (token->val[1])
                    {
                    case '=': // <=
                        t = LESS_EQ;
                        break;
                    default: // <
                        t = LESS;
                        break;
                    }
                    break;
                case '>': // more
                    switch (token->val[1])
                    {
                    case '=': // >=
                        t = MORE_EQ;
                        break;
                    default: // >
                        t = MORE;
                        break;
                    }
                    break;
                }
                // end of switch token->val[0]

                addTypeToToken(t, token);
                addRowToToken(row, token);
                actualState = START;
                tokenFound = 1;
                break;
                // end of default case
            }
            // end of switch(c)
            break;
            // end of LESS_MORE_S

        case EQ_S:
            switch (c)
            {
            case '=':
                if (strlen(token->val) < 3)
                { // = or ==
                    addCharToToken(c, token);
                }
                else
                {
                    error(1, token);
                    ; // ==== and more
                }
                break;
            default:
                switch (strlen(token->val))
                {
                case 1: // =
                    unGetC(c);
                    addRowToToken(row, token);
                    addTypeToToken(EQ, token);
                    actualState = START;
                    tokenFound = 1;
                    break;
                case 3: // ===
                    unGetC(c);
                    addRowToToken(row, token);
                    addTypeToToken(THREE_EQ, token);
                    actualState = START;
                    tokenFound = 1;
                    break;
                default: // ==
                    error(1, token);
                    ;
                }
                // end of switch strlen(token->val)
                break;
            }
            // end of switch(c)
            break;
            // end of EQ_S

        case NUM_S:
            switch (isdigit(c))
            {       // first will be for sure digit (default case above)
            case 1: // TRUE
                addCharToToken(c, token);
                break;
            case 0: // FALSE
                switch (tolower(c))
                {
                case '.': // float
                    if (strchr(token->val, '.') != NULL)
                    { // 144.4.2
                        error(1, token);
                    }
                    t = FLOAT;
                    actualState = NUM_NEEDED_S;
                    addCharToToken(c, token);
                    break;
                case 'e':
                    if (token->val[strlen(token->val) - 1] == '.' || strchr(token->val, 'e') != NULL)
                    { // 142e23e or 142.e
                        error(1, token);
                    }
                    actualState = NUM_OR_PLUSMIN_NEEDED_S;
                    addCharToToken(c, token);
                    break;
                case '+':
                    if (token->val[strlen(token->val) - 1] != 'e')
                    { // must be e before +
                        error(1, token);
                    }
                    actualState = NUM_NEEDED_S;
                    addCharToToken(c, token);
                    break;
                case '-':
                    if (token->val[strlen(token->val) - 1] != 'e')
                    { // must be e before -
                        error(1, token);
                    }
                    actualState = NUM_NEEDED_S;
                    addCharToToken(c, token);
                    break;
                default:
                    if (isOkAfterNum(c))
                    { // check if valid char is after number expression
                        actualState = START;
                        addRowToToken(row, token);
                        addTypeToToken(t, token);
                        tokenFound = 1;
                        unGetC(c);
                    }
                    else
                    {
                        error(2, token);
                    }
                    break;
                }
                break;
            }
            // end of switch(isdigit(c))
            break;
            // end of case NUM_S

        case NUM_NEEDED_S:
            if (isdigit(c))
            {
                addCharToToken(c, token);
                actualState = NUM_S;
            }
            else
            {
                error(1, token);
            }

            break;
            // end of case NUM_NEEDED_S

        case NUM_OR_PLUSMIN_NEEDED_S:
            if (isdigit(c) || c == '+' || c == '-')
            {
                addCharToToken(c, token);
                actualState = NUM_S;
            }
            else
            {
                error(1, token);
            }
            break;
            // end of case NUM_OR_PLUSMIN_NEEDED_S

        case ID_S:
            switch (checkId(c, token))
            {
            case 1: // char which can be added to id
                addCharToToken(c, token);
                break;
            case 0:
            {
                // checking keywords match
                if (!strcmp(token->val, "float"))
                {
                    if (questionMark)
                    {
                        addQuestionMark(token);
                    }
                    t = FLOAT_TYPE;
                }
                else if (!strcmp(token->val, "int"))
                {
                    if (questionMark)
                    {
                        addQuestionMark(token);
                    }
                    t = INT_TYPE;
                }
                else if (!strcmp(token->val, "string"))
                {
                    if (questionMark)
                    {
                        addQuestionMark(token);
                    }
                    t = STRING_TYPE;
                }
                else if (questionMark)
                { // question mark can be only before float, int or string
                    error(2, token);
                }
                else
                {
                    if (!strcmp(token->val, "else"))
                    {
                        t = ELSE;
                    }
                    else if (!strcmp(token->val, "function"))
                    {
                        t = FUNCTION;
                    }
                    else if (!strcmp(token->val, "if"))
                    {
                        t = IF;
                    }
                    else if (!strcmp(token->val, "null"))
                    {
                        t = NULL_KEYWORD;
                    }
                    else if (!strcmp(token->val, "return"))
                    {
                        t = RETURN;
                    }
                    else if (!strcmp(token->val, "void"))
                    {
                        t = VOID;
                    }
                    else if (!strcmp(token->val, "while"))
                    {
                        t = WHILE;
                    }
                    else
                    {
                        t = ID;
                    }
                }
                // end of switch by token value

                addTypeToToken(t, token);
                addRowToToken(row, token);
                tokenFound = 1;
                questionMark = 0;
                break;
                // end of case 0
            }
            }
            // end of switch(checkId(c))
            break;
            // end of ID_S

        case VAR_ID_S:
            switch (checkId(c, token))
            {
            case 1:
                addCharToToken(c, token);
                break;
            case 0:
                addTypeToToken(t, token);
                addRowToToken(row, token);
                tokenFound = 1;
                break;
                // end of switch(checkId(c))
            }
            break;
            // end of VAR_ID_S

        case SLASH_S:
            switch (c)
            {
            case '/': // "//"
                if (!skipLineComment())
                {
                    unGetC(c);
                }
                actualState = START;
                break;
            case '*': // /*
                skipBlockComment(token);
                actualState = START;
                break;
            default: // "/"
                t = SLASH;
                unGetC(c);
                addCharToToken('/', token);
                addTypeToToken(t, token);
                addRowToToken(row, token);
                tokenFound = 1;
                break;
                // end of switch(c)
            }
            break;
            // end of case SLASH_S

        default:
            break;
            // end of switch by actualState
        }
    }

    return token;
}

/*** End of scanner.c ***/
