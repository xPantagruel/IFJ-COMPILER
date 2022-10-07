enum type {
    NOT_DEFINED, // initial type
    VAR_ID, // $variable
    SLASH // / (divide)
};

enum state {
    START,
    SLASH_S,
    VAR_ID_S
};
typedef struct token {
    char *val;
    int row;
    enum type t; 
} Token;

/** global variable -> so we know if we passed prolog */
int prologFound = 0;

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



