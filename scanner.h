enum type {
    zatvorka,
    string,
    err
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



