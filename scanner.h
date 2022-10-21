/**
 * @file scanner.h
 *
 * @author Martin Kubička (xkubic45)
 * @author Matěj Macek (xmacek27)
 * @author Dominik Petřík (xpetri25)
 * @author Lukáš Zítko (xzitko00)
 *
 * @brief Declatarions of enums, functions and variables used in scanner.c.
 *
 * @date 2022-10-08
 */
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** types that token->t can acquire */
enum type
{
    NOT_DEFINED,  // initial type
    VAR_ID,       // $variable
    SLASH,        // / (divide)
    EOF_T,        // EOF
    L_PAR,        // (
    R_PAR,        // )
    L_CPAR,       // {
    R_CPAR,       // }
    SEMICOL,      // ;
    COMMA,        // ,
    PLUS,         // +
    MINUS,        // -
    DOT,          // .
    MUL,          // *
    ID,           // write, reads..
    ELSE,         // else
    FLOAT_TYPE,   // float
    FUNCTION,     // function
    IF,           // if
    INT_TYPE,     // int
    NULL_KEYWORD, // null
    RETURN,       // return
    STRING_TYPE,  // string
    VOID,         // void
    WHILE,        // while
    INT,          // 123e-1
    FLOAT,        // 1.32e+32
    EQ,           // =
    THREE_EQ,     // ===
    LESS,         // <
    MORE,         // >
    LESS_EQ,      // <=
    MORE_EQ,      // >=
    NOT_EQ,       // !==
    STRING,       // "string \x1F"
    COLON         // :
};                // end of enum type

/** states of FSM */
enum state
{
    START,
    SLASH_S,
    VAR_ID_S,
    ID_S,
    NUM_S,
    NUM_NEEDED_S,
    NUM_OR_PLUSMIN_NEEDED_S,
    EQ_S,
    LESS_MORE_S,
    NOT_EQ_S,
    STRING_S
};
/** token struct */
typedef struct token
{
    char *val;   // value from stdin
    int row;     // row from stdin
    enum type t; // type of value
} Token;

/** global variable -> so we know if we passed prolog */
static int prologFound = 0;

/** global variable for counting rows from stdin */
static int row = 0;

/** global variable which helps us to know if question mark will be added */
static int questionMark = 0;

/**
 * @brief Function which validates prolog.
          If proglog isn't valid -> exit(2).
 */
void getProlog();

/**
 * @brief When parser wants next token, this function will be called.
 *
 * @return Token* pointer to token
 */
Token *getToken();

/**
 * @brief Function which initialize token.
 *
 * @return Token* pointer to created token
 */
Token *tokenInit();

/**
 * @brief Function which adds char to token value.
 *
 * @param c char which will be added to token value
 * @param token pointer to token
 */
void addCharToToken(int c, Token *token);

/**
 * @brief Function which adds row to token->row.
 *
 * @param row number of row where token is (stdin)
 * @param token pointer to token
 */
void addRowToToken(int row, Token *token);

/**
 * @brief Function which adds type to token->type.
 *
 * @param t type which will be added to token
 * @param token pointer to token
 */
void addTypeToToken(enum type t, Token *token);

/**
 * @brief Function for creating token where token->value consists of one char.
 *
 * @param token pointer to token
 * @param c char which will be added to token value
 * @param row number of row where token is (stdin)
 * @param t type which will be added to token
 */
void setOneCharToken(Token *token, int c, int row, enum type t);

/**
 * @brief Optimalized ungetc() function for this project.
          Differences: There is if statement which checks
                       if last char from stdin was '\n'.
                       If yes -> we don't want to put this char back to stdin
                                 because there will be issue with row counting.
                       If no -> put char back to stdin.
 *
 * @param c char which will or won't be send back stdin
 */
void unGetC(int c);

/**
 * @brief If block comment is detected, this function will throw it away.
 *
 * @param token pointer to token -> error function
 */
void skipBlockComment(Token *token);

/**
 * @brief If line comment is detected, this function will throw it away.
 *
 * @return int 0 -> if comment is like this: //commentEOF otherwise 1
 */
int skipLineComment();

/**
 * @brief Function which validate if next char meets the conditions for ID.
 *
 * @param c char which will be validated
 * @param token pointer to token -> error function
 *
 * @return int 1 if char meets the conditions for ID otherwise 0
 */
int checkId(int c, Token *token);

/**
 * @brief Function which validate if next char meets the conditions for NUMBER.
 *
 * @param c char which will be validated
 *
 * @return int 1 if char meets the conditions for NUMBER otherwise 0
 */
int isOkAfterNum(int c);

/**
 * @brief Function that checks if there was a closing prolog.
 *
 * @return int 1 if closing prolog was detected otherwise 0
 */
int checkClosingProlog();

/**
 * @brief Function that adds question mark to the beginning of token->val.
 *
 * @param token pointer to token
 */
void addQuestionMark(Token *token);

/**
 * @brief Function which frees a memory and calls exit(errID)
 *
 * @param errID int of exit number
 * @param token pointer to token which will be freed
 */
void error(int errID, Token *token);

/**
 * @brief Token destructor.
 *
 * @param token pointer to token
 */
void dtorToken(Token *token);

#endif

/*** End of scanner.h ***/
