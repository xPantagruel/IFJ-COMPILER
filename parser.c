#include "scanner.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// example how to call token
int main() {
    Token *token = getToken();
    while (!strcmp(token->val, "EOF")) {
        printf("%s\n", token->val);
        free(token);
        token = getToken();
    }
    printf("%s\n", token->val); //print EOF
}

