enum type {
    firstType,
    secondType
};

typedef struct token {
    char *val;
    int row;
    enum type t; 
} Token;

Token getToken();
