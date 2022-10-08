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
    EOL_T, // \n
    ID,
    ELSE,
    FLOAT, // type
    FUNCTION,
    IF,
    INT, // type
    NULL_T,
    RETURN,
    STRING, // type
    VOID,
    WHILE,
    LESS, // <
    MORE, // >
    LESS_EQ, // <= 
    MORE_EQ, // >=
    EQ, // =
    THREE_EQ, // ===
    NOT_EQ // !==
};

enum state {
    START,
    SLASH_S,
    VAR_ID_S,
    ID_S
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

void setOneCharToken(Token *token, char c, int row, enum type t);
