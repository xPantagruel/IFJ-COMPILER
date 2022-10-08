enum type {
    NOT_DEFINED, // initial type
    VAR_ID, // $variable
    SLASH, // / (divide)
    EOF_T, // EOF
    L_PAR, // (
    R_PAR, // )
    L_CPAR, // {
    R_CPAR, // }
    SEMICOL, // ;
    COMMA, // ,
    PLUS, // +
    MINUS, // -
    DOT, // .
    MUL, // *
    ID,
    ELSE,
    FLOAT_TYPE, // float
    FUNCTION,
    IF,
    INT_TYPE, // type
    NULL_KEYWORD,
    RETURN,
    STRING_TYPE, // type
    VOID,
    WHILE,
    INT,
    FLOAT, // 1.32e+32
    EQ, // =
    THREE_EQ, // ===
    LESS, // <
    MORE, // >
    LESS_EQ, // <= 
    MORE_EQ, // >=
    NOT_EQ, // !==
    STRING, // "string \x1F"
    COLON // :
};

enum state {
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
    STRING_S,
    GET_HEX_S,
    GET_OKT_S
};
typedef struct token {
    char *val;
    int row;
    enum type t; 
} Token;

/** global variable -> so we know if we passed prolog */
int prologFound = 0;

int row = 1;

/**
 * @brief Function which validates prolog.
 *        If proglog isn't valid -> exit(2).
 */
void getProlog();

Token *getToken(); 

Token *tokenInit();

void addCharToToken(int c, Token *token);

void addRowToToken(int row, Token *token);

void addTypeToToken(enum type t, Token *token);

int checkIdVar(int c);

int skipLineComment();

void skipBlockComment();

void setOneCharToken(Token *token, int c, int row, enum type t);

int isOkAfterNum(int c);

int checkClosingProlog();

void unGetC(int c);

